all:
	@$(MAKE) -C src build
clean:
	@rm -rf obj
	@rm -f soush