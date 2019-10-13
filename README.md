# C++ Templating Engine
This task is to provide a reusable C++ library of a [templating engine](https://en.wikipedia.org/wiki/Template_processor). 
The template is provided to the templating engine as an input stream and it is compiled into 
an internal representation detailed below. With this compiled template and the user-provided
data, the templating engine generates the document to an output stream.

## Template Format
The template is a text document which is embedded with _template commands_, which instructs
the templating engine how to inject the data into the template. All template commands are
delimited by `{{` and `}}`. If an opening brace is intended to be printed out instead of 
being part of a template command, it can be prepended by a backslash. That is, ￿`\{` is
interpreted as a printable opening brace. Furthermore, two consecutive backslash (i.e. `\\`) is
considered to be a single printable backslash.

In addition, the leading and trailing spaces of the template commands are trimmed for the
sake of better readability of the command. For example, both template commands `{{command text}}` and
`{{ command text  }}` are regarded as identical template commands.

Finally, if a template command is not closed completely (e.g. `{{command}`), this is treated
as an ordinary text instead of a template command.

### Print Command
In order to print the value of a single-valued parameter named `varname`, one can use the template
command `{{varname}}`. Here are a few notes for this command:
1. The parameter name must not start with `#` or `/` as they are used to denote other types of
template commands.
2. The mechanism for escape characters (i.e. `\{` and `\\`) is also applicable to parameter
names.
3. If the value is not set for the parameter, the whole template command, including the pair
`{{` and `}}`, is printed.

### Loop Command
The command `{{#LOOP list item}}` initiates a loop over the content of the multi-valued 
`list` with the iterator identifier named `item`. Such loop is ended by the command 
`{{/LOOP}}`. For instance, if the template is
```
{{#LOOP list item}}
This is a {{item}}.
{{/LOOP}}
```
and ``list`` has the content `["apple", "banana", "citrus"]`. The generated document would be
```
This is a apple.
This is a banana.
This is a citrus.
```
If the list parameter turns out to be a single-valued string, the loop command considers
it to be a list comprising of this string value only.

Notice that nested loop is supported. For example, if the template is
```
{{#LOOP outerList outerItem}}
Outer begins.
{{#LOOP innerList innerItem}}
outerItem is {{outerItem}}.
innerItem is {{innerItem}}.
{{/LOOP}}
Outer ends.
{{/LOOP}}
```
and `outerList` and `innerList` are `["outer item 1", "outer item 2"]` and `["inner item 1", "inner item 2", "inner item 3"]`
respectively, the generated document is
```
Outer begins.
outerItem is outer item 1.
innerItem is inner item 1.
outerItem is outer item 1.
innerItem is inner item 2.
outerItem is outer item 1.
innerItem is inner item 3.
Outer ends.
Outer begins.
outerItem is outer item 2.
innerItem is inner item 1.
outerItem is outer item 2.
innerItem is inner item 2.
outerItem is outer item 2.
innerItem is inner item 3.
Outer ends.
```

A few more notes about the loop commands are:
1. If the item name collides with another parameter name, inside the loop, the item value
overrides that parameter value.
2. If the `{{/LOOP}}` command is missed for a loop, the opening `{{#LOOP}}` command is then
treated as a plain text.

## Templating Engine API
### Template Class
All classes are under the namespace `templatingengine` to reduce the chance of name collisions. The facade of the whole library is
the class `Template`, which can be included from `Template.h`. It is constructed by an input stream with the following constructor:
```cpptools
Template::Template(std::istream& inputStream)
```
The constructor transforms the `inputStream` into its own internal representation, which is
further elaborated in later part of this document.

After that, the document can be generated with the following method:
```cpptools
void Template::generateDocument(const ParameterSet& parameters, std::ostream& outputStream) const
```
where
* `ParameterSet` is the data structure which holds the parameter values. This is briefly described
in later part of this section.
* `outputStream` is the output stream which contains the generated document.

### ParameterSet
Parameter values are provided to the `Template` class via the `ParameterSet`, which is a
key-value pair store. The header file for `ParameterSet` is `ParameterSet.h`. 

To assign a single string value to a parameter, this can be
conveniently achieved as follows:
```cpptools
parameterSet["variableName"] = "Value for variable 1";
```
On the other hand, to assign a list of strings to a parameter, one of the simplest way is:
```cpptools
parameterSet["listName"] = {"Item 1", "Item 2", "Item 3"};
```
The right-hand side value can not only be an initializer list, but also any iterable string
containers. If you would like to assign an empty to the parameter, you can simply assign
an empty initializer list to it.

## Design of Templating Engine Library
###Tokenization
The input stream is firstly divided into a list of  _tokens_ by the `TokenListBuilder` class
which can be found in the source subdirectory `builders`. A token is an abstracted
representation of a substring in the input stream. All types of tokens are defined in
the source subdirectory `tokens` and they are subclasses of `TokenBase`, which can be found
in `TokenBase.h`.

Since the template is a plain text document embedded with template commands, the texts
outside the command is represented by the `PlainTextToken` class. Each type of command is
represented by its corresponding token type class. Some template commands such as the print
command exist on their own while others such as the loop command must co-exist with other
template commands. The former type of tokens are sub-classes of `StandaloneTokenBase` while
the others are sub-classes of `NonStandaloneTokenBase`. If a token is an opening command
such as `{{#LOOP}}`, it is a sub-class of `TokenBlockOpeningBase`, which has a method to
return the corresponding token type which ends the corresponding block of tokens so that
the templating engine can more conveniently locate the end of the token block.

###Renderers
The content of a token or a token block is generated by the corresponding type of _renderer_ object.
After building the token list from the input stream of the template, the constructor of the
`Template` class utilizes an `RendererChainBuilder` object to construct the corresponding
collection of renderers, which is known as a _renderer chain_.

In order to accommodate the nested command block structure, the [Composite](https://en.wikipedia.org/wiki/Composite_pattern) 
pattern is adopted in the design of renderer classes. All renderers are subclasses of 
`RendererBase`. Contents of standalone tokens like `PlainTextToken` or `VariableToken` are
generated by the corresponding `PlainTextRenderer` and `VariableRenderer` objects respectively.
On the other hand, content generation of a content block is taken care by an object of a
sub-class of `RendererChain`. Inside an `RendererChain` object, it is a list of `RendererBase`
object. Notice that `RendererChain` is a sub-class of `RendererBase`. That means, we can
embed one renderer chain inside another renderer chain. Take `LoopRenderer` as an example
application of such design. A `LoopRenderer` object consists of a renderer chain which 
corresponds to the body of the loop. By having another `LoopRenderer` inside this renderer
chain, we can form a nested loop. Similarly, in the future, we can embed other kinds of
renderer chains like the ones corresponding to if-statements or any complex combination
of renderer chains easily under this class structure.

###ParameterSet
The challenge of implementing the `ParameterSet` is to support multiple value type for a parameter.
Here, multiple does not only mean the current data types (string and string list), but this
also requires that adding new data types should not require too much code changes.

####Data Type Design
To tackle the aforementioned data type problem, a new collection of data types, which are
defined in the source subdirectory `dataTypes`, are introduced. All new concrete data types
are sub-classes of the `ValueBase` class. A `string` value is eventually stored inside a
`StringValue` object and the `ParameterSet` stores a `ValueBase` shared pointer of this
object (to be elaborated more later in this document). Similarly, a list of strings are stored
inside a `ValueList` object, which wraps a list of `ValueBase` shared pointer. Since the
`ParameterSet` stores `ValueBase` shared pointers instead of concrete class objects, in general,
it requires no changes in `ParameterSet` if we introduce new data type.

Moreover, the `ValueBase` class has a pure abstract method `void printTo(std::ostream& os) const`.
By overriding this method in the concrete data type sub-class, the renderer is automatically
able to print out the value to the output stream. Thus, introducing a new data type in general
does not require much changes in the renderer classes.

####ValueHandler
Although the introduction of the `ValueBase` class provides a way to store multiple types
of values in `ParameterSet`, the `ParameterSet` is still inconvenient to be used. For instance,
assigning a value to a parameter still requiring explicit conversion of the C++ data types
into `ValueBase` shared pointers. This does not only make the data assignment statements
cumbersome, but also requires the user to have more detailed knowledge of the concrete
sub-classes of `ValueBase`. On the other hand, templating engine API users only want
to assign their familiar C++ values to `ParameterSet` without the knowledge of different
concrete `ValueBase` classes. Their usage of the API should be loosely coupled with changes
of these `ValueBase` classes.

Therefore, instead of purely storing `ValueBase` shared pointers, the `ParameterSet` is defined as `std::unordered_map<std::string, ValueHandler>`.
A `ValueHandler` holds a `ValueBase` shared pointer and simplifies the assignment of data
to the `ParameterSet`. When a `ValueHandler` is assigned a value, it automatically converts
that piece of data into the corresponding `ValueBase` object and then stores the `ValueBase`
shared pointer.

On the other hand, the `ValueHander` can be casted to a `ValueBase` shared pointer. As mentioned
above, the `ValueBase` class has pure abstract method `printTo` for printing the data to
an output stream. Renderers can then conveniently print the value independent of the concrete
implementation of the data types.

##Directory Structure
The source code consists of the following directories:

|Directory|Purpose                                                                     |
|---------|----------------------------------------------------------------------------|
|`include`| Header files for the templating engine library                             |
|`src`    | Cpp files for the concrete implementation of the templating engine classes |
|`tests`  | Source files for unit tests                                                |

##Build
Cmake file is provided for building the static library `libtemplatingengine.a` and the
executables of the unit tests. This requires cmake version 2.8.2 or above. In order to
build the templating engine library and the unit tests, use the following command
```
cmake --build <build directory> --target all -- -j 4
```
All the build artifacts are then generated into the `<build directory>`. If you only want to
build the library, invoke the following command
```
cmake --build <build directory> --target templatingengine -- -j 4
```
Similarly, you can replace `templatingengine` above by the name of the unit test executable
name to build a particular unit test executable.

Finally, the following command is used for clearing up the build directory:
```
cmake --build <build directory> --target clean -- -j 4
``` 

##Testing
Unit tests are implemented in the sources in the source directory `tests`. The adopted
testing framework is the [Google test](https://github.com/google/googletest). One executable
corresponds to the unit test of one class. You can directly run the executable to run all
the tests of that class or use the `--gtest_filter` to select a subset of tests to be run.
Details can be found by using the `-h` parameter of the unit test executable.

You do not need to install Google test by yourself before building the tests but the cmake
automatically downloads the source files for you. 
