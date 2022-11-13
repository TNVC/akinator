#include "Tree.h"

#include "ConsoleArgsUtils.h"
#include "Akinator.h"
#include "Settings.h"

int main(const int argc, const char * const argv[])
{
  //db::ResourceBundle bundle = Bundle;

  Settings settings = {};

  if (parseConsoleArgs(argc, argv, &settings))
    return 0;

  setSettings(&settings);

  if (init())
    return 0;

  start();

  return 0;
}

