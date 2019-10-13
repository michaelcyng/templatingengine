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

Another note is that if the item name collides with another parameter name, inside the loop, the item value
overrides that parameter value.

## Templating Engine API
### Template Class
All classes are under the namespace `templatingengine`. The facade of the whole library is
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
containers. If you would like to assign an empty list 
