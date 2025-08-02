_5:

expansion almost working
with space from -1 to 0 to 1
this test case fails echo kk""hh''
tokens should only be separated by metacharacters
fixed ^C issue, it was missing one line after the wait in execute command
the prompt/history issue is also fixed (issue documented in wiki)
fixed lex_parser to support cat|ls (pipe with no space); pipe is a metacharacter it should create a new token

_6:

    added ft_isspace to libft (TODO: update my libft on github)
    fixed norm errors in libft
    updated and tested count_tokens
    add -Wall -Wextra -Werror to makefile
    protected ft_strncmp in echo from nulls

- TODO: figure out why sometimes execvp is not called
- TODO: update token count to match the updated logic -> DONE
- TODO: fix splitting on unqouted metacharacters only -> DONE
- TODO: expander                                      -> DONE
- TODO: add << here_doc to tokeniser/expansion logic
- TODO: fixxxx
        ataan@ataani-spectre:~/mighty_chick/chicken_6$ fff >>
        -bash: syntax error near unexpected token `newline'
- TODO: write ft_strndup                               -> NO NEED, user substr with start=0
- TODO: fix cat '|' | ls                               -> NO NEED matches bash
- TODO: t_Token create_token(e_TokenType type remove type
- TODO: clean_exit();



TODO:
here_doc
redirections
Signal + 128
check norm
Cleanup
check for not allowed functions
isatty
expander

_7: *not integrated with zaid
    fix $?, 		*result = ft_strjoin(*result, status);
    fix echo $PWD (verify token was not calling create_token coz of wrong if condition)
    *updated call buit-in to set grand.chicken.status
    *return after calling builtin (also needed to have correct status)
    added at end of execute():
        grand->chicken.status = WEXITSTATUS(grand->chicken.status);

    handle sigint in main loop
        if (g_received_signal == SIGINT)
        {
            grand.chicken.status = 130;
            g_received_signal = 0;            
        }
        
    TODO: fix man echo
    *TODO: check if cd hgh should return 1 not -1 --> DONE
    TODO: check this in chicken.c
        if (ast && (ast->type == NODE_COMMAND || ast->type == NODE_PIPE || ast->type == NODE_BUILTIN) /* && grand.chicken.status != 13*/)
        {
            execute(ast, &grand);
        }
    TODO: change PWD to get directory from env variable

_8: 
    fix pwd.c and 
        in call_builtin use: chkn_pwd(node->args, grand->env.envp);
        update pwd in chicken.h