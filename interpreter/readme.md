# Grammar

*NOTE:* Terminals will be bold before it

The deleted rules are 6 ,11,14,17,27,28,29.

## Original Grammar

|       Symbol        |                                                     Definition                                                      | Needs Modification |
| ------------------- | ------------------------------------------------------------------------------------------------------------------- | ------------------ |
| program             | *type-specifier* **main** **(** *declaration-list* **)** {*declaration-list* *statement-list*}                      |                    |
| declaration-list    | *declaration-list* *declaration* &#124; *declaration*                                                               | YES (Left-Rec.)    |
| declaration         | *var-declaration*                                                                                                   |                    |
| var-declaration     | *type-specifie*r **ID** **;** &#124; *type-specifier* **ID** \[**NUM**\]                                            | YES (factoring)    |
| type-specifier      | **int** &#124; **float**                                                                                            |                    |
| DELETED 6           | DELETED                                                                                                             |                    |
| params              | *param-list* &#124; *void*                                                                                          |                    |
| param-list          | *param-list* **,** *param* &#124; *param*                                                                           | YES (Left-Rec.)    |
| param               | *type-specifier* **ID** &#124; *type-specifier* **ID** **\[** **\]**                                                | YES (factoring)    |
| compound-stmt       | {*statement-list*}                                                                                                  |                    |
| DELETED 11          | DELETED                                                                                                             |                    |
| statement-list      | *statement-list* *statement* &#124; **empty**                                                                       | YES (factoring)    |
| statement           | *assignment-stmt*  &#124; *compound-stmt* &#124; *selection-stmt* &#124; *iteration-stmt*                           |                    |
| DELETED 14          | DELETED                                                                                                             |                    |
| selection-statement | **if** **(** *expression* **)** *statement* &#124; **if** **(** *expression* **)** *statement* **else** *statement* | YES (Left-Rec.)    |
| iteration-statement | **while** **(** *expression* **)** *statement*                                                                      |                    |
| DELETED 17          | DELETED                                                                                                             |                    |
| assignment-stmt     | *var* **=** *expression*                                                                                            |                    |
| var                 | **ID** &#124; **ID** **\[** *expression* **\]**                                                                     | YES (factoring)    |
| expression          | *expression* *relop* *additive-expression* &#124; *additive-expression*                                             | YES (Left-Rec.)    |
| relop               | **<=** &#124; **<** &#124; **>** &#124; **>=** &#124; **==** &#124; **!=**                                          | YES (factoring)    |
| additive-expression | *additive-expression* *addop* *term* &#124; *addop*                                                                 | YES (Left-Rec.)    |
| addop               | **+** &#124; **-**                                                                                                  |                    |
| term                | *term* *mulop* *factor* &#124; *factor*                                                                             | YES (Left-Rec.)    |
| mulop               | **\*** &#124; **/**                                                                                                 |                    |
| factor              | **(** *expression* **)** &#124; *var* &#124; **NUM**                                                                |                    |
| DELETED 27, 28, 29  | DELETED                                                                                                             |                    |

## Modified Grammar(LL1)

|          Symbol          |                                             Definition                                             |
| ------------------------ | -------------------------------------------------------------------------------------------------- |
| program                  | *type-specifier* **main** **(** *declaration-list* **)** {*declaration-list* *statement-list*}     |
| declaration-list         | *declaration* *declaration-list-Tail*                                                              |
| declaration-list-Tail    | *declaration* *declaration-list-Tail* &#124; **empty**                                             |
| declaration              | *var-declaration*                                                                                  |
| var-declaration          | *type-specifier* **ID** *var-declaration-Tail*                                                     |
| var-declaration-Tail     | **;** &#124; \[**NUM**\] **;**                                                                     |
| type-specifier           | **int** &#124; **float**                                                                           |
| DELETED 6                | DELETED                                                                                            |
| params                   | *param-list* &#124; **void**                                                                       |
| param-list               | *param* *param-list-tail*                                                                          |
| param-list-Tail          | **,** *param* *param-list* &#124; **empty**                                                        |
| param                    | *type-specifier* **ID** *param-Tail*                                                               |
| param-Tail               | **empty** &#124; **\[** **\]**                                                                     |
| compound-stmt            | {*statement-list*}                                                                                 |
| DELETED 11               | DELETED                                                                                            |
| statement-list           | *statement-list-Tail*                                                                              |
| statement-list-Tail      | *statement* *statement-list-Tail* &#124; **empty**                                                 |
| statement                | *assignment-stmt*  &#124; *compound-stmt* &#124; *selection-stmt* &#124; *iteration-stmt*          |
| DELETED 14               | DELETED                                                                                            |
| selection-statement      | **if** **(** *expression* **)** *statement* *selection-statement-Tail*                             |
| selection-statement-Tail | **empty** &#124; **else** *statement*                                                              |
| iteration-statement      | **while** **(** *expression* **)** *statement*                                                     |
| DELETED 17               | DELETED                                                                                            |
| assignment-stmt          | *var* **=** *expression*;                                                                           |
| var                      | **ID**  *var-Tail*                                                                                 |
| var-Tail                 | **empty** &#124; **\[** *expression* **\]**                                                        |
| expression               | *additive-expression* *expression-Tail*                                                            |
| expression-Tail          | *relop* *additive-expression* *expression-Tail* &#124; **empty**                                   |
| relop                    | **<** *relop-Tail* &#124; **>** *relop-Tail* &#124; **==** &#124; **!=** |
| relop-Tail               | **=** &#124; **empty**                                                                             |
| additive-expression      | *term* *additive-expression-Tail*                                                                  |
| additive-expression-Tail | *addop* *term* *additive-expression-Tail* &#124; **empty**                                         |
| addop                    | **+** &#124; **-**                                                                                 |
| term                     | *factor* *term-Tail*                                                                               |
| term-Tail                | *mulop* *factor* *term-Tail* &#124; **empty**                                                      |
| mulop                    | **\*** &#124; **/**                                                                                |
| factor                   | **(** *expression* **)** &#124; *var* &#124; **NUM**                                               |
| DELETED 27, 28, 29       | DELETED                                                                                            |


### Implementation Issues and decisions:
1. *program* rule
   1. **problem**: The rule doesn't match the intended start of the file. (int main() {})
   2. **decision**: Changed the rule to work with the input we want that takes (int main() { })
2. *assignment-stmt* 
   1. **problem**: assignment statements needed to be terminated with a semicolon.
   2. **decision**: added semicolon to assignment-stmt(rule 18).



### Handling syntax error:

To handle syntax error we kept track of all the tokens read and their line number. On encountering an error, the program terminates after printing the token expected, the token found, and the line number at which the error happened.

### Problems in the implementation:

Selection and iterative statements are not executing correctly.

### Sample run
![sample run](sample%20run.png)