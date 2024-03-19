#include "comms.h"

const float cMath::Pi			= 3.14159265358979323846f;
const float cMath::TwoPi		= 2.0f * cMath::Pi;
const float cMath::HalfPi		= 0.5f * cMath::Pi;
const float cMath::QuarterPi	= 0.25f * cMath::Pi;

const float cMath::RadsPerDeg = cMath::Pi / 180.0f;
const float cMath::DegsPerRad = 180.0f / cMath::Pi;

const double cMath::dPi = 3.1415926535897932384626433832795;
const double cMath::dTwoPi = 2.0 * cMath::dPi;
const double cMath::dHalfPi = 0.5 * cMath::dPi;

const double cMath::dRadsPerDeg = cMath::dPi / 180.0;
const double cMath::dDegsPerRad = 180.0 / cMath::dPi;

const float cMath::SecsPerMs = 0.001f;
const float cMath::MsPerSec = 1000.0f;

const float cMath::Epsilon = 0.001f;
const float cMath::SpaceEpsilon = 0.1f;
const float cMath::Infinity = 1e30f;

const double cMath::dEpsilon = 1.0e-6;

const float cMath::Sqrt1Over2 = 0.70710678118654752440f;
const float cMath::Sqrt1Over3 = 0.57735026918962576450f;

const double cMath::DoubleEpsilon = double(0x1); // 4.9406564584124654e-324
const double cMath::DoubleMinValue = double(-1.7976931348623157e+308);
const double cMath::DoubleMaxValue = double(1.7976931348623157e+308);
const double cMath::DoubleNaN = double(0xfff8000000000000); // -1.#IND
const double cMath::DoubleNegativeInfinity = double(0xfff0000000000000); // -1.#INF
const double cMath::DoublePositiveInfinity = double(0x7ff0000000000000); // 1.#INF

const float cMath::FloatMaxValue = 3.402823466e+38f;
const float cMath::FloatMinValue = 1.175494351e-38f;

const int cMath::IntMinValue = int(0x80000000);
const int cMath::IntMaxValue = int(0x7fffffff);