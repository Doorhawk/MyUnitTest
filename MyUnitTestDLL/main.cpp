#include "unittest.h"

TEST(first) {
	int a = 0;
	int b = 0;
	EQVAL_UNSTOP(2 + 2, 3);
	EQVAL_UNSTOP(2 + 2, 4);
	EQVAL_UNSTOP(2 + 2, 3);
	EQVAL_UNSTOP(2 + 2, 4);
	EQVAL_UNSTOP(2 + 2, 3);
	EQVAL_UNSTOP(2 + 2, 4);
	EQVAL_UNSTOP(2 + 2, 3);
	EQVAL_UNSTOP(2 + 2, 4);
	throw std::invalid_argument("INVALID");
	EQVAL_UNSTOP(2 + 2, 3);
	EQVAL_UNSTOP(2 + 2, 4);
	EQVAL_UNSTOP(2 + 2, 3);
	EQVAL_UNSTOP(2 + 2, 4);
	EQVAL_UNSTOP(2 + 2, 3);
}


TEST_GROUP(g2, sec) {
	EQVAL(2 + 2, 4);
}
TEST_GROUP(g2, trid) {
	int answ = 4;
	EQVAL(2 + 3, answ);
}
TEST_GROUP(g2, trid1) {
	int answ = 4;
	EQVAL(2 + 2, answ);
}
TEST(third2) {
	int answ = 4;
	EQVAL(2 + 2, answ);
}
TEST(third3) {
	int answ = 4;
	EQVAL(2 + 2, answ);
	EQVAL(2 + 3, answ);
}

DEFINE_FIXATURE(mega,
	int x = 0;
int y = 10;
)

TEST(testwithfix) {
	EQVAL(mega.x, 0);
	EQVAL(mega.y, 10);
}
TEST(testwithfix1) {
	EQVAL(mega.x, 0);
	EQVAL(mega.y, 10);
}
TEST(testwithfix2) {
	CHECK_UNSTOP(mega.x == 0);
	CHECK_UNSTOP(mega.y == 10);
	CHECK_UNSTOP(mega.y == 9);
	CHECK_UNSTOP(mega.y == 10);
	CHECK_UNSTOP(mega.y == 9);
	CHECK_UNSTOP(mega.y == 10);
	CHECK_UNSTOP(mega.y == 9);
	CHECK_UNSTOP(mega.y == 10);
	CHECK(mega.y == 10);
	CHECK(mega.y == 10);
	CHECK(mega.y == 10);
	CHECK(mega.y == 10);
}

TEST_PARAM_CASE(testcase, int a, int b, int c) {
	EQVAL(a + b, c);
}
TEST_PARAM(testcase, 1, 2, 3);
TEST_PARAM(testcase, 1, 2, 4);
TEST_PARAM(testcase, 1, 2, 5);
TEST_PARAM(testcase, 1, 2, 6);

TEST_PARAM_GROUP(g1,testcase, 2, 2, 4);
TEST_PARAM_GROUP(g1,testcase, 2, 2, 4);
TEST_PARAM_GROUP(g1,testcase, 2, 2, 4);

struct Vector
{
	Vector(double x, double y) :x(x), y(y) {}
	double x, y;
	bool operator==(const Vector& vec) const {
		if (x == vec.x && y == vec.y)
			return true;
		return false;
	}
	bool operator!=(const Vector& vec) const {
		if (x != vec.x || y != vec.y)
			return true;
		return false;
	}

};
std::ostream& operator<<(std::ostream& os, const Vector& v) {
	return os << "(" << v.x << ", " << v.y << ")";
}

TEST_PARAM_CASE(testcase2, Vector vec1, Vector vec2) {
	EQVAL(vec1, vec2);
}

TEST_PARAM(testcase2, Vector(1, 1), Vector(1, 1));
TEST_PARAM(testcase2, Vector(1, 1), Vector(2, 2));

TEST_PARAM_GROUP(g1,testcase2, Vector(1, 1), Vector(2, 2));


TEST_DATA(datatestm, (std::vector<int>{2, 2, 2, 2, 5}), int val) {
	EQVAL(val, 2);
}

TEST_DATA_GROUP(g2,datatestmgroup, (std::vector<int>{1, 2, 3, 4, 5}), int val) {
	EQVAL(val, 2);
}


TEST_GROUP(g1,EPS) {
	EQVAL_EPS(0, 0.0001, 0.00001);
	EQVAL_EPS(0, 0.0001, 0.00001);
	EQVAL_EPS(0, 0.0001, 0.00001);
	EQVAL_EPS(0, 0.0001, 0.00001);
	EQVAL_EPS(0, 0.0001, 0.00001);
	EQVAL_EPS(0, 0.0001, 0.00001);
	EQVAL_EPS(0, 0.0001, 0.1);
}
TEST(EPS1) {
	EQVAL_EPS(0, 0.0001, 0.0001);
}
TEST(EPS2) {
	EQVAL_EPS(0, 0.0001, 0.001);
}
TEST(EPS3) {
	EQVAL_EPS(0, -0.0001, -0.001);
}


class Point {
public:
	double x, y;
	Point(double x, double y) :x(x), y(y) {};
	double getX() { return x; }
	double getY() { return y; }
	void rotate(const Point& center, double angle) {
		
		x -= center.x;
		y -= center.y;
		double newx = cos(angle) * x - sin(angle) * y;
		double newy = sin(angle) * x + cos(angle) * y;
		x = newx + center.x;
		y = newy + center.y;

	}
	bool operator!=(const Point& poi) {
		if (x != poi.x || y != poi.y) {
			return true;
		}
		return false;
	}
};

std::ostream& operator<<(std::ostream& os, const Point& v) {
	return os << "(" << v.x << ", " << v.y << ")";
}

TEST_GROUP(point, point_rotate) {
	Point p(0, 0);
	Point center(1, 0);
	p.rotate(center, acos(-1) / 2);
	p.rotate(center, acos(-1) / 2);
	p.rotate(center, acos(-1) / 2);
	p.rotate(center, acos(-1) / 2);
	EQVAL_EPS(p.getX(), 0, -1e-10);
	EQVAL_EPS(p.getY(), 0, 1e-10);

	p = Point(1, 0);
	p.rotate(Point(0, 0), acos(-1.) / 6);
	double answ = sqrt(3.0) / 2;
	EQVAL(3.0, answ);
	EQVAL(p.getY(), 0.5);
}

void gerExpect() {
	throw std::invalid_argument("error poblem");
}

TEST_GROUP(exepct, MSGgood) {
	EXPECT_THROW_MSG(gerExpect(), std::invalid_argument, "error poblem");
}
TEST_GROUP(exepct, MSGbed) {
	EXPECT_THROW_MSG(gerExpect(), std::invalid_argument, "error pem");
}
TEST_GROUP(exepct, good1) {
	EXPECT_THROW(gerExpect(), std::invalid_argument);
}
TEST_GROUP(exepct, bed1) {
	EXPECT_THROW(gerExpect(), std::runtime_error);
}


int main() {
	TEST_LOG_OFF();
	RUN_ALL_TESTS();
	//RUN_TESTS_GROUP(g2);
	//RUN_TESTS_GROUP(g1);
	//RUN_TESTS_GROUP(exepct);


	/*for (int i = 0; i < 100; i++) {
		std::cout << "\033[0;" + std::to_string(i) + "m" << "AAAAAAAAAAAAA - " << RESET_COLOR << i << std::endl;
	}*/


	return 0;
}