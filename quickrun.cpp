#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <filesystem>
#include <cstdlib>
#include <exception>
#include <vector>

#define OUTPUT_FILE "/tmp/qruntmp"

class parsing_error: public std::exception
{
  virtual const char* what() const throw()
  {
    return "Could not parse file.";
  }
} parsing_error;

std::string preprocess_line(const std::string &line) {
  std::string final_command {};

  for (int i=0;i<static_cast<int>(line.length());i++) {
    if (line[i] == '%' && line[i+1] == '%') {
      i += 1;
      final_command += OUTPUT_FILE;
      continue;
    }

    final_command += line[i];
  }

  return final_command;
}

std::string parse_line(const std::string &line) {
  std::string final_command {};

  bool found_comment { false };
  bool command_started { false };
  for (int i=0;i<static_cast<int>(line.length());i++) {
    if (line[i] == ' ' && command_started == false)
      continue;
    
    final_command += line[i];

    if (final_command == "\/\/") {
      final_command = "";
      found_comment = true;
      command_started = true;
    }
  }

  if (found_comment == false)
    throw parsing_error;

  return final_command;
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
  
  std::string command{};
  first_line = preprocess_line(first_line);
  
  try {
    command = parse_line(first_line);
  } catch (std::exception& e) {
    std::cout << e.what() << "\n";
    return 1;
  }

  std::system(command.c_str());

  std::system(OUTPUT_FILE);
  
  return 0;
}
