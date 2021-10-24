CFLAGS = -Wall -Werror -Wextra

NAME = minishell

LIBFT_DIR = libft

LIBFT = libft.a

HEADER = includes/minishell.h

SRC =	main.c  \
		src/env/update_shlvl.c \
		src/env/update_env_var.c \
		src/env/save_env_utils.c \
		src/env/save_env.c \
		src/env/env_var_check.c \
		src/get_next_line_eof.c \
		src/signals.c \
		src/check_input.c \
		src/io_stream.c \
		src/process_input.c \
		src/parser.c \
		src/scanner/scanner.c \
		src/scanner/syntax_error.c \
		src/tokenizer/get_cmd_2d_array.c \
		src/tokenizer/tokenizer.c \
		src/tokenizer/create_tokens.c \
		src/expansions/expand_variable.c \
		src/expansions/expander.c \
		src/expansions/expansion_checks.c \
		src/expansions/extract_value.c \
		src/expansions/special_parameters.c \
		src/lexer/lexer.c \
		src/lexer/set_token_id.c \
		src/lexer/set_rd.c \
		src/lexer/set_args.c \
		src/executer/execute.c \
		src/executer/run_shell_cmd.c \
		src/executer/execute_pipes.c \
		src/executer/redirections_out.c \
		src/executer/redirections_in.c \
		src/executer/redirections_io_stream.c \
		src/executer/errors.c \
		src/executer/exit_status.c \
		src/executer/get_paths.c \
		src/executer/prep_execute.c \
		src/builtin/ft_echo.c \
		src/builtin/ft_pwd.c \
		src/builtin/ft_cd.c \
		src/builtin/ft_env.c \
		src/builtin/ft_exit.c \
		src/builtin/ft_export.c \
		src/builtin/ft_unset.c \
		src/builtin_utils/ft_export_utils.c \
		src/builtin_utils/ft_cd_utils.c \
		src/helper_functions/free.c \
		src/helper_functions/prep_input.c \
		src/helper_functions/identify.c

GREEN   = \x1b[32;01m
RESET   = \x1b[0m

all:
	$(MAKE) -C $(LIBFT_DIR) bonus
	$(MAKE) $(NAME)

$(NAME): $(SRC) $(HEADER)
	$(CC) $(CFLAGS) $(SRC) $(LIBFT_DIR)/$(LIBFT) -o $(NAME)
	@echo "$(GREEN) 🐚 minishell executable created $(RESET)"

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	@echo "$(GREEN) 🐚 minishell clean $(RESET)"

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "$(GREEN) 🐚 minishell fcleaned $(RESET)"

re: fclean all

.PHONY: all clean fclean re
