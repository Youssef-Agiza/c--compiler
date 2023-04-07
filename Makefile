all:
	@$(MAKE) lexical-analyzer --no-print-directory
	@$(MAKE) semantic-analyzer --no-print-directory

lexical-analyzer:
	@cd "lexical analyzer" && $(MAKE) --no-print-directory

semantic-analyzer:
	@cd "semantic analyzer" && $(MAKE) --no-print-directory
