#include "tokenizer.h"
#include "general.h"
#include "arena.h"

// ====
// Main
// ====

int main(int argc, char **argv){
	if(argc <= 1){
		fprintf(stderr, "[FATAL 0x%04X]: Not enough arguements.\n", 0x0001);
		exit(EXIT_FAILURE);
	}
	arenaInit();
	size_t len = 0;
	char *buff = readFile(argv[1], &len);

	// execution
	// 	TODO: tokenize the text
	TokenList *list = tokenize(buff);
	tokenListPrint(list);
	// 	TODO: convert the tokens into an AST
	// 	TODO: emit bytecode based on AST

	// buffer can be freed after emition of bytecode
	free(buff);

	//	TODO: execute bytecode
	//	requires cortex-vm to be completed and the inclusion of the cortex-vm.h header

	arenaDestroy();
	return EXIT_SUCCESS;
}
