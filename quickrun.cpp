#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <filesystem>
#include <cstdlib>
#include <exception>
#include <vector>
#include <cstring>

#define OUTPUT_FILE "/tmp/qruntmp"

class parsing_error: public std::exception {
  virtual const char* what() const throw() {
    return "Could not parse file.";
  }
} parsing_error;

class QuickrunContext {
public:
  std::string output_file;
  std::string input_file;
  std::string line;
  std::string command;
};

void preprocess_line(QuickrunContext &context) {
  std::string final_command {};

  for (int i=0;i<static_cast<int>(context.line.length());i++) {
    if (context.line.substr(i, 3) == "%O%") {
      i += 2;
      final_command += context.output_file;
      continue;
    }

    if (context.line.substr(i, 3) == "%I%") {
      i += 2;
      final_command += context.input_file;
      continue;
    }

    final_command += context.line[i];
  }

  context.line = final_command;
}

void parse_line(QuickrunContext &context) {
  std::string final_command {};

  bool found_comment { false };
  bool command_started { false };
  for (int i=0;i<static_cast<int>(context.line.length());i++) {
    if (context.line[i] == ' ' && command_started == false)
      continue;
    
    final_command += context.line[i];

    // checks if is comment
    if (final_command == "\/\/") {
      final_command = "";
      found_comment = true;
      command_started = true;
    }
  }

  if (found_comment == false)
    throw parsing_error;

  context.command = final_command;
}

int main(int argc, char* argv[]) {
  // checks that a file has been entered
  if (argc == 1) {
    std::cout << "Quickrun v0.1.0 by Berserkware\n\n";
    std::cout << "Quickrun a file by passing the filepath to this program.\n\n";
    std::cout << "Add Quickrun to a file by adding a comment at the start"
      "\nof the file that contains the command to compile the program.\n";

    std::cout << "\nPreprocessor Options:"
      "\n - %O% : Gets replaced with a automatic output file (usually in /tmp)."
      "\n   You need to make the compiler output this, because it is the file that"
      "\n   qrun executes after running your command."
      "\n - %I% : Get replaced with the name of the file. Allows you to rename the"
      "\n   file without having to change the qrun command.\n";
    return 0;
  }

  std::filesystem::path path = std::filesystem::absolute(argv[1]);

  std::ifstream file{ path };

  if (!file) {
    std::cerr << "Could not open file.\n";
    return 1;
  }

  QuickrunContext context;
  context.output_file = OUTPUT_FILE;
  context.input_file = path;
  std::getline(file, context.line);
  context.command = "";
  
  preprocess_line(context);
  
  try {
    parse_line(context);
  } catch (std::exception& e) {
    std::cout << e.what() << "\n";
    return 1;
  }
  
  std::system(context.command.c_str());

  // adds the args to the command
  std::string final_command {};
  final_command += context.output_file.c_str();
  for (int i = 2; i < argc; i++) {
    final_command += " ";
    final_command += argv[i];
  }

  std::system(final_command.c_str());
  
  return 0;
}
