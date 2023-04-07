all:
	@$(MAKE) lexical-analyzer --no-print-directory
	@$(MAKE) semantic-analyzer --no-print-directory

lexical-analyzer:
	@$(MAKE) "./lexical analyzer" --no-print-directory

semantic-analyzer:
	@$(MAKE) "./semantic analyzer" --no-print-directory
