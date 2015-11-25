Infix Expression Evaluator
for a school assignment

Assumptions and Limitations

The program assumes staying within the limits of a normal integer type. There is no overflow
checking.
Example: 1000000000 * 3
This gives an incorrect answer with no warning.

There is one situation where space between operators must affect evaluation. This is handled as
follows:
There is a reading precedence that is different from operator precedence. This is to deal with the
possible ambiguities concerning:
	- unary negative
	-- unary decrement
	- binary subtraction
The parser knows when a binary operator is required, and when a binary operator is forbidden.
If a binary operator is required, it will see the binary subtraction operator first. If a binary operator is
forbidden, it will see the unary decrement operator first, and if that is not present, then it will see the
unary negative operator.
Examples:
	--- -----2 (decrement negative decrement decrement negative 2 = 3)
	--------2 (decrement decrement decrement decrement 2 = -2)
	4--3 (4 subtract negative 3 = 7)
	4---3 (4 subtract decrement 3 = 2)
	4----3 (4 subtract decrement negative 3 = 8)
	4---- ---3 (4 subtract decrement negative decrement negative 3 = 1)
It also affects how the following operators are interpreted, but not how they are evaluated, and
not whether they are valid:
	++ unary increment
	+ binary addition
For these, it would only make a difference in what error message you might expect to see. But
there are no ambiguous evaluations or ambiguous interpretations in whether they are valid.
	9*+++5 (9 multiply increment <error: Binary operator not allowed @ char: 4>...)
	9*6++5 (9 multiply 6 add <error: Binary operator not allowed @ char: 4>...)

The example error messages in the specification mostly describe what is not allowed. The rules
in this program are not programmed from the perspective of what is not allowed, but instead from the
perspective of what is required. So most of the error messages match this perspective.
Example:
	15+3 2
Instead of the error that describes what is not allowed:
	"Two operands in a row @ char: 5"
The error describes what is required:
	"Binary operator required @ char: 5"

The division by zero error indicates the location of the division operator (since the zero could
come from a complex calculation without a specific location).
Negative exponents are only allowed with a base of -1, 0, 1.

Efficiency of Algorithms

The Token struct holds either an integer (operand) or an operator and has a boolean member to
know which one it is and a boolean member to know whether an operator is unary.
The check_unary function is to set a value to is_unary, designed to be called only during
construction. The efficiency is O(1) since the number of different possible unary operators is constant
and not an input to the program.
The operator> function compares integers if both tokens being compared are integers (this
functionality is not used in this program) and compares precedence if both tokens are operators. If
tokens are not of the same type, it throws an exception. The efficiency is O(1).
The other comparison operators use the operator> function.
The Evaluator class holds an expression as a string and has the capability to evaluate this
expression.
The read_int function reads an integer out of the expression wherever the cursor is. The
efficiency is O(n) on the number of digits in the integer.
The eat_white function moves the cursor past whitespace in the expression. The efficiency is
O(n) on the amount of whitespace.The cursor_str function makes a string from a cursor position suitable for error messages. The
efficiency is O(1).
The make_tokens function parses the expression to find the operators and operands and turn
them into tokens. The efficiency is O(n) on the length of the expression.
The process_operator function figures out whether operators need to be evaluated, according to
the order of operations. The efficiency is usually O(1), but could be O(n) on the number of operators in
the operator stack.
The eval_tokens function processes the tokens, separating them into stacks of operands and
operators and evaluating them when needed. The efficiency is O(n) on the number of tokens.
The evaluate_one_operator function handles the evaluation of a single operator. The efficiency
is O(1).
The eval functions are wrappers for the whole evaluation process. The efficiency is O(n) on the
length of the expression. (None of the O(n) functions are called inside another O(n) function unless the
n is limited with the number of times the function is called. For example, the process_operator
function, that could be O(n), is called inside the eval_tokens function, but the total n in all of the calls
to process_operator cannot exceed the n of eval_tokens.)
The way I can think of to make the evaluation more efficient would be to implement short
circuit evaluation on the boolean operators (0 && ??, 1 || ??). This might be easier if stacks were not
required for the assignment, since the operands for the binary operators have to be already on the stack
when the operator is evaluated. But this would rarely be a significant increase in efficiency, and not
using stacks might decrease the efficiency in other cases.