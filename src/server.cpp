#include "server.h"
#include "pam.h"

static std::string okResponse =
        "HTTP/1.0 200 OK\n"
        "Content-type: text/html\n"
        "\n";

static std::string okCSSResponse =
        "HTTP/1.0 200 OK\n"
        "Content-type: text/css\n"
        "\n";

static std::string badRequestResponse =
        "HTTP/1.0 400 Bad Request\n"
        "Content-type: text/html\n"
        "\n"
        + FileReader::readFile(std::string(ERROR_PAGES_DIR).append("400.html"));

static std::string notAuthorizedResponse =
        "HTTP/1.0 401 Not Authorized\n"
        "Content-type: text/html\n"
        "WWW-Authenticate: Basic realm=\"Router Authentication\"\n"
        "Content-Length: 0\n"
        "\n"
        + FileReader::readFile(std::string(ERROR_PAGES_DIR).append("401.html"));

static std::string notFoundResponseTemplate =
        "HTTP/1.0 404 Not Found\n"
        "Content-type: text/html\n"
        "\n"
        + FileReader::readFile(std::string(ERROR_PAGES_DIR).append("404.html"));

static std::string badMethodResponseTemplate =
        "HTTP/1.0 501 Method Not Implemented\n"
        "Content-type: text/html\n"
        "\n"
        + FileReader::readFile(std::string(ERROR_PAGES_DIR).append("501.html"));

static void cleanUpChildProcess(int signalNumber)
{
  int status;
  wait(&status);
}

static bool validAuthData(char* data)
{
  std::string credentials = Base64::decode(std::string(data));
  std::string login = credentials.substr(0, credentials.find(":"));
  std::string password = credentials.substr(credentials.find(":")+1, std::string::npos);
  return authenticate(login.c_str(), password.c_str());
}

static void handleGet(int connectionFileDescriptor, const char* page)
{
  begin:
  struct ServerModule* module = NULL;
  if(*page == '/')
  {
    char moduleFileName[64];
    if(StringHelper::hasEnding(std::string(page), ".css"))
    {
      snprintf(moduleFileName, sizeof(moduleFileName), std::string(MODULES_DIR).append("%s").c_str(), "css.so");
    }
    else
    {
      snprintf(moduleFileName, sizeof(moduleFileName), std::string(MODULES_DIR).append("%s.so").c_str(), page+1);
    }
    module = moduleOpen(moduleFileName);
  }
  if(module == NULL)
  {
    page = "/index";
    goto begin;
    char response[1024];
    snprintf(response, sizeof(response), notFoundResponseTemplate.c_str(), page);
    write(connectionFileDescriptor, response, strlen(response));
  }
  else
  {
    if(StringHelper::hasEnding(std::string(page), ".css"))
    {
      write(connectionFileDescriptor, okCSSResponse.c_str(), okCSSResponse.length());
    }
    else
    {
      write(connectionFileDescriptor, okResponse.c_str(), okResponse.length());
    }
    std::map<std::string, std::string> m;
    if(StringHelper::hasEnding(std::string(page), ".css"))
    {
      m.insert(std::pair<std::string, std::string>("addr", std::string(page+1)));
    }
    (*module->generateFunction)(connectionFileDescriptor, m);
    moduleClose(module);
  }
}

static void handlePost(int connectionFileDescriptor, const char* page, std::map<std::string, std::string> postVars)
{
  struct ServerModule* module = NULL;
  if(*page == '/' && strchr(page+1, '/') == NULL)
  {
    char moduleFileName[64];
    snprintf(moduleFileName, sizeof(moduleFileName), std::string(MODULES_DIR).append("%s.so").c_str(), page+1);
    module = moduleOpen(moduleFileName);
  }
  if(module == NULL)
  {
    char response[1024];
    snprintf(response, sizeof(response), notFoundResponseTemplate.c_str(), page);
    write(connectionFileDescriptor, response, strlen(response));
  }
  else
  {
    write(connectionFileDescriptor, okResponse.c_str(), okResponse.length());
    (*module->generateFunction)(connectionFileDescriptor, postVars);
    moduleClose(module);
  }
}

static void handleConnection(int connectionFileDescriptor)
{
  char buffer[256];
  memset(buffer, 0, sizeof(buffer));
  //char authorizationData[128];
  //bool authDataPresent = false;
  std::string readText(buffer);
  ssize_t bytesRead;
  bytesRead = read(connectionFileDescriptor, buffer, sizeof(buffer) - 1);
  if(bytesRead > 0)
  {
    char method[sizeof(buffer)];
    char url[sizeof(buffer)];
    char protocol[sizeof(buffer)];
    buffer[bytesRead] = '\0';
    sscanf(buffer, "%s %s %s", method, url, protocol);
      /*
      if(std::string(buffer).find("Authorization: Basic") != std::string::npos)
      {
          authDataPresent = true;
          sscanf(buffer + std::string(buffer).find("Authorization: Basic "), "Authorization: Basic %s\n", authorizationData);
      }
      */

    readText.append(std::string(buffer));
    do
    {
      memset(buffer, 0, sizeof(buffer));
      bytesRead = read(connectionFileDescriptor, buffer, sizeof(buffer));
      readText.append(std::string(buffer));
    }
    while(bytesRead == sizeof(buffer));

    /*
    if(authDataPresent == false || !validAuthData(authorizationData))
    {
      write(connectionFileDescriptor, notAuthorizedResponse.c_str(), notAuthorizedResponse.length());
      close(connectionFileDescriptor);
      return;
    }
    else
    {
    */
      if(strcmp(protocol, "HTTP/1.0") && strcmp(protocol, "HTTP/1.1")) // if not http/1.0 or http/1.1
      {
        write(connectionFileDescriptor, badRequestResponse.c_str(), badRequestResponse.length());
        close(connectionFileDescriptor);
      }
      else if(strcmp(method, "GET")) // if not GET
      {
        if(!strcmp(method, "POST")) // if POST
        {
          int postLength = 0;
          std::size_t found = readText.find("Content-Length: ");
          sscanf(readText.substr(found, readText.npos).c_str(), "Content-Length: %d\r\n", &postLength);
          std::string postVars = readText.substr(readText.find("\r\n\r\n")+4, readText.npos);
          std::map<std::string, std::string> postVariables;
          std::vector<std::string> tokens = StringHelper::explode(postVars, '&');
          std::for_each(tokens.begin(), tokens.end(), [&postVariables](std::string & s){
            std::vector<std::string> pairs = StringHelper::explode(s, '=');
            if(pairs.size() > 1)
            {
              postVariables.insert(std::pair<std::string, std::string>(pairs.at(0), pairs.at(1)));
            }
          });

          handlePost(connectionFileDescriptor, url, postVariables);
        }
        else
        {
          char response[1024];
          snprintf(response, sizeof(response), badMethodResponseTemplate.c_str(), badMethodResponseTemplate.length());
          write(connectionFileDescriptor, response, strlen(response));
          close(connectionFileDescriptor);
        }
      }
      else
      {
        handleGet(connectionFileDescriptor, url);
      }
    // }
  }
  else if(bytesRead == 0)
  { }
  else
  {
    systemError("read");
    close(connectionFileDescriptor);
  }
}

void serverRun(in_addr localAddress, uint16_t port)
{
  struct sockaddr_in socketAddress;
  struct sigaction sigchldAction;
  int serverSocket;
  int rval;

  memset(&sigchldAction, 0, sizeof(sigchldAction));
  sigchldAction.sa_handler = &cleanUpChildProcess;
  sigaction(SIGCHLD, &sigchldAction, NULL);
  serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(serverSocket == -1)
  {
    systemError("socket");
  }
  int on = 1;
  rval = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (const char*) &on, sizeof(on));
  if(rval == -1)
  {
    systemError("setsockopt");
  }
  memset(&socketAddress, 0, sizeof(socketAddress));
  socketAddress.sin_family = AF_INET;
  socketAddress.sin_port = port;
  socketAddress.sin_addr = localAddress;
  rval = bind(serverSocket, (struct sockaddr *) &socketAddress, sizeof(socketAddress));
  if(rval != 0)
  {
    systemError("bind");
  }
  rval = listen(serverSocket, 3);
  if(rval != 0)
  {
    systemError("listen");
  }
  while(1)
  {
    struct sockaddr_in remoteAddress;
    socklen_t addressLength;
    pid_t childPid;
    int connection;

    addressLength = sizeof(remoteAddress);
    connection = accept(serverSocket, (struct sockaddr *) &remoteAddress, &addressLength);
    if(connection == -1)
    {
      if(errno == EINTR)
      {
        continue;
      }
      else
      {
        systemError("accept");
      }
    }
    childPid = fork();
    if(childPid == 0)
    {
      close(STDIN_FILENO);
      close(STDOUT_FILENO);
      //close(STDERR_FILENO);
      close(serverSocket);
      handleConnection(connection);
      close(connection);
      exit(0);
    }
    else if(childPid > 0)
    {
      close(connection);
    }
    else
    {
      systemError("fork");
    }
  }
}
