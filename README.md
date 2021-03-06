# paramset: A Parameter Management Utility for C++

## Overview
- Header-only library for managing parameters
  - Easy definition of parameters
  - Integrated support for JSON config file and command line arguments
  - Type-safe access to obtained parameters

## Requirements
- C++11 compiler
- Depends on the following header-only libraries:
  - [JSON for Modern C++](https://github.com/nlohmann/json)
  - [cmdline](https://github.com/tanakh/cmdline)
  - [Catch](https://github.com/philsquared/Catch) (for testing)


## Running Example

### Preparation

```sh
git submodule update --init --recursive
```

### Source code

example/example_paramset.cpp:

```cpp
// 1. include paramset
#include "paramset.hpp"

int main(int argc, char* argv[]){
	// 2. define parameters
	//   - name, default[, json_path][, long_opt, short_opt, description, [required_in_cmd]]
	const paramset::definitions defs = {
		// default_value should be a string, int, double or bool
		{"txt", "Hello, paramset!", {"text"}, "strarg", 's', "string argument", false},
		// required_in_cmd is optional (default: false)
		{"cnt", 1, {"count"}, "intarg", 'i', "integer argument"},
		// set short_option to 0 if unnecessary
		{"rad", 2.3, {"radius"}, "doublearg", 0, "double argument"},
		// parameter only available in JSON config file
		{"flg", true, {"path", "to", "flag"}},
		// parameter only available in command line arguments
		{"conf", "", "config", 'c', "config file path"},
		// you can also define constants
		{"PI", 3.14}
	};
	// 3. initialize parameters with default values
	paramset::manager pm(defs);
	try{
		// 4. overwrite parameters with config file and command line arguments as follows:
		//   - command line argument named "config" (default: "") specifies config file path
		//   - if config file exists, overwrite parameters with it
		//   - then, overwrite with command line arguments
		//   - finally, at least 2 (default: 0) unnamed command line arguments are required
		pm.load(argc, argv, "config", 2);

		// 5. use parameter
		std::string txt = pm["txt"];
		int cnt = pm["cnt"];
		std::cout << "text: " << txt << std::endl;
		std::cout << "count: " << cnt << std::endl;

		// 6. directly use parameter in function call
		std::cout << "flag: " << pm.get<bool>("flg") << std::endl;
		std::cout << "circumference: " <<
				2 * pm.get<double>("rad") * pm.get<double>("PI") << std::endl;

		// 7. use the rest of command line arguments
		for(const auto& p: pm.rest)
			std::cout << "rest: " << p.as<std::string>() << std::endl;

		// 8. you can update or add parameters if needed
		pm["cnt"] = 0;
		pm["prob"] = 0.5;
		std::cout << "new count: " << pm.get<int>("cnt") << std::endl;
		std::cout << "probability: " << pm.get<double>("prob") << std::endl << std::endl;
	}
	catch(const std::exception& e){
		std::cerr << "an error occured: " << e.what() << std::endl;
		exit(1);
	}
	return 0;
}
```

example/config.json:

```json
{
	"text": "JSON",
	"count": 100,
	"radius": 10.0,
	"path": {
		"to": {
			"flag": false
		}
	}
}
```

### Run

```sh
$ cd example
$ make example
```

### Result

```sh
g++ -std=c++11 -Wall -I .. -I ../external/cmdline -I ../external/json/src paramset.cpp -o paramset
./paramset foo bar
text: Hello, paramset!
count: 1
flag: 1
circumference: 14.444
rest: foo
rest: bar
new count: 0
probability: 0.5

./paramset -c config.json foo bar
text: JSON
count: 100
flag: 0
circumference: 62.8
rest: foo
rest: bar
new count: 0
probability: 0.5

./paramset -c config.json -s 'command line argument' -i 123 --doublearg 456.7 foo bar baz
text: command line argument
count: 123
flag: 0
circumference: 2868.08
rest: foo
rest: bar
rest: baz
new count: 0
probability: 0.5
```

## Use in Your Project
```sh
$ cp external/cmdline/cmdline.h external/json/src/json.hpp paramset.hpp {your include directory}
```
