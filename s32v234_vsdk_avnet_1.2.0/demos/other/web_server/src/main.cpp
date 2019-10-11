#include "webserver.hpp"

void onInitMethod(std::string* name, std::string* value)
{
  // name compare with HTML tag
  if((*name).compare("max_color") == 0)
  {
    *value = "127";
    return;
  }
  if((*name).compare("stream_en") == 0)
  {
    *value = "true";
    return;
  }
}

void onUpdateMethod(std::string* name, std::string* value)
{
  /*
     *   Do something when some shared value was updated ! This is only event, the value was updated locally in server
     *   So use this only as trigger of your logic depends on updated value name !!!
     */
}

void onCommandMethod(std::string* command)
{
  // Do something with own command !
}

int main(int argc, char* argv[])
{
  try
  {
    // Check command line arguments.
    if(argc != 3)
    {
      std::cerr << "Usage: server <address> <port>\n";
      std::cerr << "  For IPv4, try:\n";
      std::cerr << "    server 0.0.0.0 7777\n";
      std::cerr << "  For IPv6, try:\n";
      std::cerr << "    server 0::0 7777\n";

      return 1;
    }

    std::cout << "Server address: " << argv[1] << std::endl;
    std::cout << "Server port: " << argv[2] << std::endl;
    std::cout << "Server is starting..." << std::endl;

    // Initialise the server.
    webserver_setup(argv[1], argv[2]);

    // Set listener for initialise value of shared value
    webserver_setOnInitEventListener(&onInitMethod);

    // Set listener if shared value is updated
    webserver_setOnUpdateEventListener(&onUpdateMethod);

    // Set listener if own command is used
    webserver_setOnCommandEventListener(&onCommandMethod);

    // Run the server until stopped.
    webserver_run();

    std::cout << "Server has started." << std::endl;
    std::cout << "Press Ctrl+C (Ctrl+Break) to exit." << std::endl << std::endl;
  }
  catch(std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
    return 1;
  }

  // Simple image buffer 320 * 180 * 3
  int   size          = 172800;
  int   frame_counter = 0;
  char* image         = (char*)malloc(size);
  for(int i = 0; i < size; i++)
  {
    image[i] = 127;
  }

  for(;;)
  {
    // Value of HTML tag "max_color", second parametr is value max_color cant be read
    int max_color = webserver_getIntValueByName("max_color", 255);

    // Generata random image data
    for(int i = 0; i < size; i += 1)
    {
      image[i] = (char)(1 + (rand() % (int)(max_color - 1 + 1)));
    }

    // Stream image buffer if server isRunning and stream is enabled
    if(webserver_isRunning())
    {
      if(webserver_getBoolValueByName("stream_en", false))
      {
        webserver_streamBuffer(image, size, "main");
        frame_counter++;
      }
    }
    else
    {
      break; // Server is not isRunning break;
    }

    // Prepare vector with some output values that hase to be shared
    std::string value = "NXP";
    webserver_addOutValue("String", value);
    webserver_addOutValue("Bool", true);
    webserver_addOutValue("Int", frame_counter);
    webserver_addOutValue("Double", 0.00125);

    // Send output values to clients
    webserver_sendOutSharedValues();
  }

  return 0;
}