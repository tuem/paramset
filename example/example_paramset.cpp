// 1. include paramset
#include "paramset.hpp"

int main(int argc, char* argv[]){
	// 2. define parameters
	//   - name, default[, json_path][, long_opt, short_opt, description, [required_in_cmd]]
	paramset::definitions defs = {
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

		// 5. read parameter
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
	}
	catch(const std::exception& e){
		std::cerr << "an error occured: " << e.what() << std::endl;
		exit(1);
	}
	return 0;
}
