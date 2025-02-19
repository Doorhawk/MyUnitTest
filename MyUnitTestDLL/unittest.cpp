
#include "pch.h"
#include "unittest.h"

namespace ut {

	Test::Test(const std::string& name, TestFun fun, const std::string& groupName)
		:name(name), fun(fun), groupName(groupName), passed(false), error("") {}
	Test::Test(const Test& other)
		: name(other.name), fun(other.fun), groupName(other.groupName), passed(other.passed), error(other.error) {}
	Test::~Test() {}
	const std::string& Test::getName() const {
		return name;
	}
	const std::string& Test::getError() const {
		return error;
	}
	const std::string& Test::getGroupName() const {
		return groupName;
	}
	bool Test::isPassed() const {
		return passed;
	}
	void Test::run() {
		try {
			fun();
			passed = true;
		}
		catch (const std::exception& e) {  // Ловит std::invalid_argument и другие стандартные исключения
			error = e.what();
			passed = false;
		}
		catch (const std::string& e) {  // Ловит throw std::string("message")
			error = e;
			passed = false;
		}
		catch (const char* e) {  // Ловит throw "message"
			error = e;
			passed = false;
		}
		catch (...) {
			error = "unknown exception";
			passed = false;
		}
	}
	void Test::addLog(std::string log) {
		testLog.push_back(log);
	}
	void Test::clearLog() {
		testLog.clear();
	}
	const std::vector<std::string>& Test::getLog() const {
		return testLog;
	}


	TestRegistry& TestRegistry::getInstance() {
		static TestRegistry instance;
		return instance;
	}
	void TestRegistry::runAll() {
		complited = 0;
		testNum = 0;
		std::cout << BLUE_COLOR << "\n\t[All]" << RESET_COLOR << ":" << "\n";
		log.out << "\n\t[All]" << ":" << "\n";
		for (auto& test : tests) {
			test.clearLog();
			test.run();
			printTestInf(test);
			testNum++;
		}
		std::cout << "\tcomplited: " << RESET_COLOR << complited << "/" << tests.size() << std::endl;
	}
	void TestRegistry::runGroup(const std::string& group) {
		complited = 0;
		std::cout << "\n\t" << BLUE_COLOR << "[" << group << "]" << RESET_COLOR << " group: " << "\n";
		log.out << "\n\t" << "[" << group << "]" << " group: " << "\n";
		int groupSize = 0;
		testNum = 0;
		for (auto& test : tests) {
			if (test.getGroupName() != group){
				testNum++;
				continue;
			}
			groupSize++;
			test.clearLog();
			test.run();
			printTestInf(test);
			testNum++;
		}
		std::cout << BLUE_COLOR << "\t" << "[" << group << "]" << RESET_COLOR << " complited: " << complited << "/" << groupSize << std::endl;
		log.out << "\t" << "[" << group << "]" << " complited: " << complited << "/" << groupSize << std::endl;
	}
	void TestRegistry::addTest(const Test& test) {
		tests.push_back(test);
	}
	void TestRegistry::printTestInf(const Test& test) {
		if (test.isPassed()) {
			std::cout << GREEN_TEXT << "[OK]\t" << RESET_COLOR << test.getName() << "\n";
			log.out << "[OK]\t" << test.getName() << "\n";
			complited++;
		}
		else {
			std::cout << RED_TEXT << "[FAIL]\t" << RESET_COLOR << test.getName() << ", Error: " + test.getError() << std::endl;
			log.out << "[FAIL]\t" << test.getName() << ", Error: " + test.getError() << std::endl;
		}
		if (isLogOn) {
			for (auto& str : test.getLog()) {
				if (str.find("OK") == 0) {
					std::cout << "\t\t" << GREEN_TEXT << "OK" << RESET_COLOR << str.substr(2) << "\n";
					log.out << "\t\t" << "OK" << str.substr(2) << "\n";
				}
				else if (str.find("FAIL") == 0) {
					std::cout << "\t\t" << RED_TEXT << "FAIL" << RESET_COLOR << str.substr(4) << "\n";
					log.out << "\t\t" << "FAIL" << str.substr(4) << "\n";
				}
				else {
					std::cout << "\t\t" << " " << str << "\n";
					log.out << "\t\t" << " " << str << "\n";
				}


			}
		}
	}
	void TestRegistry::TestLogOnOff(bool is) {
		isLogOn = is;
	}
	Test& TestRegistry::getThisTest() {
		return tests[testNum];
	}
	TestRegistry::TestRegistry() :complited(0) {}
	TestRegistry::~TestRegistry() {}
	


	SimpleLogger::SimpleLogger() {
		createLogDirectory();
		out.open("test/TestLog.txt", std::ios::app);
		out <<"\n--------------------------------------------------------------------------------------------------------\n";
		out <<  "-----------------------------------------"<<getCurrentTime()<<"-----------------------------------------\n";
		out <<  "--------------------------------------------------------------------------------------------------------\n\n";
	}
	SimpleLogger::~SimpleLogger() {
		out.close();
	}
	std::string SimpleLogger::getCurrentTime() {
		auto time = std::chrono::system_clock::now();
		std::time_t now_time = std::chrono::system_clock::to_time_t(time);
		std::tm local_tm;
		localtime_s(&local_tm, &now_time);
		std::ostringstream oss;
		oss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");
		return oss.str();
	}
	void SimpleLogger::createLogDirectory() {
		std::filesystem::path logDir{ "test" };
		if (!std::filesystem::exists(logDir) && !std::filesystem::create_directory(logDir)) {
			std::cerr << "Error creating log folder!" << std::endl;
		}
	}
}