#ifndef MEASURE_HPP
#define MEASURE_HPP

#include <iostream>
#include <opencv2/core.hpp>

using uint8 = unsigned char;

namespace my {

	class Point
	{
	public:
		double x;
		double y;

		Point(double& _x, double& _y) : x(_x), y(_y) {}
		Point(const cv::Point& cvpt) :x{ static_cast<double>(cvpt.x) }, y{ static_cast<double>(cvpt.y) } {}
		Point() : x(0.0), y(0.0) {}

		inline bool operator<(const Point& p) const
		{
			return x < p.x || (x == p.x && y < p.y);
		}
		inline void operator=(cv::Point& cvpt)
		{
			this->x = cvpt.x;
			this->y = cvpt.y;
		}
		static std::vector<my::Point> fromCVptArray(std::vector<cv::Point>& CVptArray);
		static std::vector<cv::Point2d> toCVptArray(std::vector<my::Point>& CVptArray);
	};

	std::vector<my::Point> Point::fromCVptArray(std::vector<cv::Point>& CVptArray)
	{
		std::vector<my::Point> mypt;
		for (int i = 0; i < CVptArray.size(); i++)
		{
			mypt.push_back(my::Point{ CVptArray[i] });
		}
		return mypt;
	}

	class Diameter
	{
	public:
		Diameter() :diameter{ 0 } {};
		Diameter(Point& pt1, Point& pt2)
		{
			this->pt1 = pt1;
			this->pt2 = pt2;
			this->diameter = hypot(pt1.x - pt2.x, pt1.y - pt2.y);
		}

		void calcd(Point& pt1, Point& pt2)
		{
			this->diameter = hypot(pt1.x - pt2.x, pt1.y - pt2.y);
		}
		static Diameter max(Diameter& a, Diameter& b)
		{
			return (a.diameter > b.diameter) ? a : b;
		}
		static Diameter max(Diameter a, Diameter b)
		{
			return (a.diameter > b.diameter) ? a : b;
		}

		double diameter;
		Point pt1;
		Point pt2;
	};

	std::vector<cv::Point2d> my::Point::toCVptArray(std::vector<my::Point>& myptArray)
	{
		std::vector<cv::Point2d> cvpt;
		for (int i = 0; i < myptArray.size(); i++)
		{
			cvpt.push_back(cv::Point2d{ myptArray[i].x,myptArray[i].y });
		}
		return cvpt;
	}

	class RotatingCalipers
	{
	public:
		//点到直线距离
		static inline double _distance(const my::Point& start, const my::Point& end, const my::Point& point);
		static inline double cross(const my::Point& O, const my::Point& A, const my::Point& B)
		{
			return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
		}
		//寻找凸包
		static std::vector<my::Point> convexHull(std::vector<my::Point> p);

		static inline double area(const my::Point& a, const my::Point& b, const my::Point& c)
		{
			return fabs((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x));
		}

		static inline double dist(const my::Point& a, const my::Point& b)
		{
			return hypot(a.x - b.x, a.y - b.y);
		}

		/*
		 * @brief	使用旋转卡壳法求出点集最远两点的距离
		 *
		 * @date	2023/6/29
		 *
		 * @param 	PontSet	轮廓点集
		 *
		 * @returns	轮廓最大距离
		*/
		static Diameter diameter(const std::vector<my::Point>& PontSet);
	};

	inline double my::RotatingCalipers::_distance(const Point& start, const Point& end, const Point& point)
	{
		double dist = (point.y - start.y) * (end.x - start.x) - (point.x - start.x) * (end.y - start.y);
		return dist;
	}

	std::vector<my::Point> my::RotatingCalipers::convexHull(std::vector<my::Point> P)
	{
		size_t n = P.size();
		size_t k = 0;
		if (n <= 3) return P;

		std::vector<my::Point> H(2 * n);
		sort(P.begin(), P.end());

		for (size_t i = 0; i < n; ++i)
		{
			while (k >= 2 && cross(H[k - 2], H[k - 1], P[i]) <= 0) k--;
			H[k++] = P[i];
		}

		for (size_t i = n - 1, t = k + 1; i > 0; --i)
		{
			while (k >= t && cross(H[k - 2], H[k - 1], P[i - 1]) <= 0) k--;
			H[k++] = P[i - 1];
		}
		H.resize(k - 1);
		return H;
	}

	Diameter my::RotatingCalipers::diameter(const std::vector<Point>& p)
	{
		std::vector<Point> h = convexHull(p);
		Diameter ptpair;
		size_t m = h.size();
		if (1 == m) return Diameter();
		if (2 == m) return Diameter(h[0], h[1]);

		size_t k = 1;
		while (area(h[m - 1], h[0], h[(k + 1) % m]) > area(h[m - 1], h[0], h[k])) k++;

		for (size_t i = 0, j = k; i <= k && j < m; i++)
		{
			ptpair = Diameter::max(ptpair, Diameter(h[i], h[j]));
			while (j < m && area(h[i], h[(i + 1) % m], h[(j + 1) % m]) > area(h[i], h[(i + 1) % m], h[j]))
			{
				ptpair = Diameter::max(ptpair, Diameter(h[i], h[(j + 1) % m]));
				j++;
			}
		}
		return ptpair;
	}
}

#endif 