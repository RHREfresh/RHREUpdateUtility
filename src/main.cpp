#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <string>
#include <chrono>
#include <thread>
#include <windows.h>
#include <shellapi.h>

#include "Error.hpp"

const int maxAttemptsPerStep = 20;
const int secondsBetweenRetries = 1;

void printHelp(char **argv) {
  printf("RHRE Update Utility v1.1\n");
  printf("RHRE Update Utility " __DATE__ " " __TIME__ "\n\n");

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

int copyDirectory(std::filesystem::path fromPath, std::filesystem::path toPath, std::filesystem::copy_options copyOptions)
{
    try
    {
        if (!std::filesystem::exists(toPath))
        {
            std::filesystem::create_directory(toPath);
        }

        for (auto const& dir_entry : std::filesystem::directory_iterator{fromPath})
        {
            std::string entry_filename = dir_entry.path().filename().string();
            std::filesystem::path target_file_path = toPath / entry_filename;

            if (std::filesystem::exists(target_file_path))
            {
                std::filesystem::remove_all(target_file_path);
            }

            std::filesystem::copy(dir_entry.path(), target_file_path, copyOptions);
        }
        return 0;
    }
    catch (std::filesystem::filesystem_error const &ex)
    {
        printf("%s", ex.what());
        return 1;
    }
}

int launchRhre(std::filesystem::path dir)
{
    for (auto const& dir_entry : std::filesystem::directory_iterator{dir})
    {
        std::string entryPath = dir_entry.path().string();
        if (stringEndsWith(entryPath, ".exe"))
        {
            ShellExecute(NULL, "open", entryPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
            return 0;
        }
    }

    return 1;
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
  while (copyAttempt <= maxAttemptsPerStep)
  {
    if (copyDirectory(extractedPath, installPath, copyOptions) == 0) break;
    if (copyAttempt < maxAttemptsPerStep)
    {
        Warn("Copying failed! Retrying...\n");
        copyAttempt++;
        std::this_thread::sleep_for(std::chrono::seconds(secondsBetweenRetries));
    }
    else
    {
        Panic("Continuously failed to copy files!");
    }
  }

  // launch rhrefresh
  printf("Launching RHREfresh...\n");
  int launchAttempt = 1;
  while (launchAttempt <= maxAttemptsPerStep)
  {
    if (launchRhre(installPath) == 0) break;
    if (launchAttempt < maxAttemptsPerStep)
    {
        Warn("Launching failed! Retrying...\n");
        launchAttempt++;
        std::this_thread::sleep_for(std::chrono::seconds(secondsBetweenRetries));
    }
    else
    {
        Panic("Continuously failed to launch RHREfresh!");
    }
  }

  // delete old file
  printf("Removing download...\n");
  int removeAttempt = 1;
  while (removeAttempt <= maxAttemptsPerStep)
  {
    if (deleteDirectory(extractedPath) == 0) break;
    if (removeAttempt < maxAttemptsPerStep)
    {
        Warn("Removing failed! Retrying...\n");
        removeAttempt++;
        std::this_thread::sleep_for(std::chrono::seconds(secondsBetweenRetries));
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
