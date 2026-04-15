#include "pch.h"

#include "WhizzLang/Core/Compiler.h"
#include "WhizzLang/Utils/FileUtils.h"

using namespace WhizzLang;

static void GetCompilerConfiguration(int argc, char** argv, CompilerConfiguration& configs);

#define WL_EXIT_ON_EXCEPTION(x) try { x; } catch(...) { std::exit(-1); }

int main(int argc, char** argv)
{
	CompilerConfiguration configs{};
	WL_EXIT_ON_EXCEPTION(GetCompilerConfiguration(argc, argv, configs));

	Compiler compiler(configs);
	compiler.Process();

	FileUtils::WriteFile(fmt::format("{}.asm", configs.SourceFile.data()), compiler.GetAssembly());
	system(fmt::format("nasm -felf64 -o {0}.o {0}.asm", configs.SourceFile.data()).c_str());
	system(fmt::format("ld -o {} {}.o", configs.OutputFile.data(), configs.SourceFile.data()).c_str());
}

void GetCompilerConfiguration(int argc, char** argv, CompilerConfiguration& configs)
{
	size_t argIndex = 1;

	while (argIndex < argc)
	{
		char* argument = argv[argIndex];

		if (strcmp(argument, "-o") == 0)
		{
			if (argc <= argIndex + 1)
			{
				fmt::println(stderr, "Error! Specify output file!");
				throw "Specify output file!";
			}

			configs.OutputFile = argv[argIndex + 1];
			argIndex += 2;
			continue;
		}

		configs.SourceFile = argument;
		argIndex++;
	}

	if (configs.SourceFile.empty())
	{
		fmt::println(stderr, "Error! Specify source file!");
		throw "Specify source file!";
	}
}
