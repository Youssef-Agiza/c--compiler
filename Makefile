all:
	@$(MAKE) lexical-analyzer --no-print-directory
	@$(MAKE) semantic-analyzer --no-print-directory

lexical-analyzer:
	@cd "lexical analyzer" && $(MAKE) --no-print-directory

semantic-analyzer:
	@cd "semantic analyzer" && $(MAKE) --no-print-directory

interpreter:
	@cd "interpreter" && $(MAKE) --no-print-directory

clean:
	@cd "lexical analyzer" && $(MAKE) clean --no-print-directory
	@cd "semantic analyzer" && $(MAKE) clean --no-print-directory
	@cd "interpreter" && $(MAKE) clean --no-print-directory