#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <filesystem>
#include <cstdlib>

bool validate_line(const std::string &line) {
  // line is invalid if empty
  if (line == "")
    return false;

  // line is invalid if doesn't start with comment.
  if (line.rfind("//", 0)) {
    return false;
  }
  
  return true;
}

std::string remove_line_comments(const std::string &line) {
  return line.substr(2);
}

int main(int argc, char* argv[]) {
  // checks that a file has been entered
  if (argc == 1) {
    std::cout << "Quickrun v0.1.0 by Berserkware\n\n";
    std::cout << "Quickrun a file by passing the filepath to this program.\n\n";
    std::cout << "Add Quickrun support to a file by adding a comment at the \nvery beginning of the file that contains the command to \nrun the file\n";
    return 0;
  }

  std::filesystem::path path = std::filesystem::absolute(argv[1]);

  std::ifstream file{ path };

  if (!file) {
    std::cerr << "Could not open file.\n";
    return 1;
  }

  std::string first_line{};
  std::getline(file, first_line);
  
  if (!validate_line(first_line)) {
    std::cerr << "Error: File could not be quickrunned.\n";
    return 1;
  }

  first_line = remove_line_comments(first_line);

  std::string final_command { first_line + " -o /tmp/qruntmp" };

  std::system(final_command.c_str());

  std::system("/tmp/qruntmp");
  
  return 0;
}
