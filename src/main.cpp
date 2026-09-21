#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <string>
#include <chrono>
#include <thread>

#include "Error.hpp"

void printHelp(char **argv) {
  printf("RHREfresh Exe Update Helper v1.0\n");
  printf("RHREfresh Exe Update Helper " __DATE__ " " __TIME__ "\n\n");

  printf("usage: %s <extracted directory> <install directory>\n", argv[0]);
  exit(1);
}

// https://stackoverflow.com/a/874160
bool stringEndsWith(std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

int copyDirectory(std::filesystem::path fromPath, std::filesystem::path toPath, std::filesystem::copy_options copyOptions)
{
    try
    {
        std::filesystem::copy(fromPath, toPath, copyOptions);
        return 0;
    }
    catch (std::filesystem::filesystem_error const &ex)
    {
        return 1;
    }
}

int deleteDirectory(std::filesystem::path dir)
{
    try
    {
        std::filesystem::remove_all(dir);
        return 0;
    }
    catch (std::filesystem::filesystem_error const &ex)
    {
        return 1;
    }
}

int launchRhre(std::filesystem::path dir)
{
    for (auto const& dir_entry : std::filesystem::directory_iterator{dir})
    {
        std::string entryPath = dir_entry.path();
        if (stringEndsWith(entryPath, ".exe"))
        {
            CreateP
            break;
        }
    }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printHelp(argv);
  }

  std::string extractedDirectory = argv[1];
  std::string installDirectory = argv[2];

  std::filesystem::path extractedPath = std::filesystem::path(extractedDirectory);
  std::filesystem::path installPath = std::filesystem::path(installDirectory);

  const auto copyOptions = std::filesystem::copy_options::overwrite_existing
                                             | std::filesystem::copy_options::recursive;

  // try to copy directory 3 times
  printf("Copying extracted update into location...\n");
  int copyAttempt = 1;
  while (copyAttempt < 4)
  {
    if (copyDirectory(extractedPath, installPath, copyOptions) == 0) break;
    if (copyAttempt < 3)
    {
        Error("Copying failed! Retrying...\n");
        copyAttempt++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    else
    {
        Panic("Continuously failed to copy files!");
    }
  }

  // delete old file
  printf("Removing download...\n");
  int removeAttempt = 1;
  while (removeAttempt < 4)
  {
    if (deleteDirectory(extractedPath) == 0) break;
    if (removeAttempt < 3)
    {
        Error("Removing failed! Retrying...\n");
        removeAttempt++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    else
    {
        Panic("Continuously failed to remove files!");
    }
  }

  printf("Launching RHREfresh...\n");
  int launchAttempt = 1;
  while (launchAttempt < 4)
  {
    if (deleteDirectory(extractedPath) == 0) break;
    if (removeAttempt < 3)
    {
        Error("Removing failed! Retrying...\n");
        removeAttempt++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    else
    {
        Panic("Continuously failed to remove files!");
    }
  }
  printf("Update complete! Enjoy RHREfresh, and keep your rhythm up! <3\n");
  std::this_thread::sleep_for(std::chrono::seconds(1));
  return 0;
}
