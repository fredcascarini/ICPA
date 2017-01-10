#include "stdafx.h"
#include <CppUnitTest.h>
#include <vector>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ICPAtests
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(TestLinearFit)
		{
			std::vector<double> input;
			for (auto i = 0.0; i < 20.0; ++i) { input.push_back(i); }

			std::vector<double> expected;
			expected.push_back(20.0);
			expected.push_back(1.0);
			expected.push_back(0.0);

			auto result = get_linear_fit(input, 0.9, 1, 0);
			Assert::IsTrue(result == expected);
		}
	};
}
