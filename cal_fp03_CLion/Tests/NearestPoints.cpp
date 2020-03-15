/*
 * NearestPoints.cpp
 */

#include <limits>
#include <thread>
#include <algorithm>
#include <cmath>
#include "NearestPoints.h"
#include "Point.h"

const double MAX_DOUBLE = std::numeric_limits<double>::max();

Result::Result(double dmin, Point p1, Point p2) {
	this->dmin = dmin;
	this->p1 = p1;
	this->p2 = p2;
}

Result::Result() {
	this->dmin = MAX_DOUBLE;
	this->p1 = Point(0,0);
	this->p2 = Point(0,0);
}

/**
 * Auxiliary functions to sort vector of points by X or Y axis.
 */
static void sortByX(vector<Point> &v, int left, int right)
{
	std::sort(v.begin( ) + left, v.begin() + right + 1,
		[](Point p, Point q){ return p.x < q.x || (p.x == q.x && p.y < q.y); });
}

static void sortByY(vector<Point> &v, int left, int right)
{
	std::sort(v.begin( ) + left, v.begin() + right + 1,
		[](Point p, Point q){ return p.y < q.y || (p.y == q.y && p.x < q.x); });
}

/**
 * Brute force algorithm O(N^2).
 */
Result nearestPoints_BF(vector<Point> &vp) {
	Result res;

	for (auto i = vp.begin();i != vp.end();i++)
    {
	    for (auto i2 = (i+1);i2 != vp.end();i2++)
        {
	        if (i->distance(*i2) < res.dmin)
            {
                res.dmin = i->distance(*i2);
                res.p1 = (*i);
                res.p2 = (*i2);
            }
        }
    }

	return res;
}

/**
 * Improved brute force algorithm, that first sorts points by X axis.
 */
Result nearestPoints_BF_SortByX(vector<Point> &vp) {
	Result res;
	sortByX(vp, 0, vp.size()-1);

    for (auto i = vp.begin();i != vp.end();i++)
    {
        for (auto i2 = (i+1);i2 != vp.end();i2++)
        {
            if (i->distance(*i2) < res.dmin)
            {
                res.dmin = i->distance(*i2);
                res.p1 = (*i);
                res.p2 = (*i2);
            }
        }
    }

	return res;
}


/**
 * Auxiliary function to find nearest points in strip, as indicated
 * in the assignment, with points sorted by Y coordinate.
 * The strip is the part of vp between indices left and right (inclusive).
 * "res" contains initially the best solution found so far.
 */
static void npByY(vector<Point> &vp, int left, int right, Result &res)
{
	for (int i = left; i <= right; i++)
    {
	    for (int i2 = i + 1; i2 <= right; i2++)
        {
	        if ((vp.at(i2).y - vp.at(i).y) > res.dmin) break;
	        else if (vp.at(i).distance(vp.at(i2)) < res.dmin)
            {
	            res.p1 = vp.at(i);
	            res.p2 = vp.at(i2);
	            res.dmin = vp.at(i).distance(vp.at(i2));
            }
        }
    }
}

bool sortPointX(const Point &left,const Point &right)
{
    return left.x < right.x;
}


bool sortPointY(const Point &left,const Point &right)
{
    return left.y < right.y;
}

/**
 * Recursive divide and conquer algorithm.
 * Finds the nearest points in "vp" between indices left and right (inclusive),
 * using at most numThreads.
 */
static Result np_DC(vector<Point> &vp, int left, int right, int numThreads) {
    // Base case of two points
	if ((right - left) == 1) return Result(vp.at(left).distance(vp.at(right)),vp.at(left),vp.at(right));

	// Base case of a single point: no solution, so distance is MAX_DOUBLE
	if (right == left) return Result(MAX_DOUBLE,vp.at(left),Point(MAX_DOUBLE,MAX_DOUBLE));

	// Divide in halves (left and right) and solve them recursively,
	// possibly in parallel (in case numThreads > 1)
	Result leftRes,rightRes;
    Result *leftResPtr = &leftRes;

	if (numThreads > 1)
    {
	    //vector<Point> *vp_ptr = &vp;
	    thread t([=](){
	        vector<Point> copy(vp);
            *leftResPtr = np_DC(copy,left,(right+left)/2,numThreads/2);
	    });
        rightRes = np_DC(vp, (right + left) / 2 + 1, right, numThreads/2);
        t.join();
    }
	else
    {
        leftRes = np_DC(vp, left, (right + left) / 2, numThreads);
        rightRes = np_DC(vp, (right + left) / 2 + 1, right, numThreads);
    }

	// Select the best solution from left and right
	Result best;
	if (leftRes.dmin < rightRes.dmin) best = leftRes;
	else best = rightRes;

	// Determine the strip area around middle point
	double middle = (vp.at(right).x+vp.at(left).x)/2;
	bool foundLeft = false,foundRight = false,pointInStrip = false;
	vector<Point>::iterator leftEdge, rightEdge;

	if (vp.at(left).x >= (middle - best.dmin)) {leftEdge = vp.begin() + left; foundLeft = true; pointInStrip = true;}
	if (vp.at(right).x <= (middle + best.dmin)) {rightEdge = vp.begin() + right; foundRight = true; pointInStrip = true;}

	if (!foundLeft || !foundRight)
    {
	    for (auto i = vp.begin() + left; i != vp.begin() + right + 1; i++)
	    {
            if (i->x >= (middle - best.dmin) && i->x <= (middle + best.dmin) && !foundLeft) {
                pointInStrip = true;
                foundLeft = true;
                leftEdge = i;
                left = i - vp.begin();
            }
            if (i->x > (middle + best.dmin) && !foundRight) {
                foundRight = true;
                rightEdge = (i - 1);
                right = (i - 1) - vp.begin();
                break;
            }
        }
    }

	// Order points in strip area by Y coordinate
	if (leftEdge != rightEdge && pointInStrip) sort(leftEdge,rightEdge+1,sortPointY);

	// Calculate nearest points in strip area (using npByY function)
	if (pointInStrip) npByY(vp,left,right,best);

	// Reorder points in strip area back by X coordinate
    if (leftEdge != rightEdge && pointInStrip) sort(leftEdge,rightEdge+1,sortPointX);

	return best;
}


/**
 * Defines the number of threads to be used.
 */
static int numThreads = 1;
void setNumThreads(int num)
{
	numThreads = num;
}

/*
 * Divide and conquer approach, single-threaded version.
 */
Result nearestPoints_DC(vector<Point> &vp) {
	sortByX(vp, 0, vp.size() -1);
	return np_DC(vp, 0, vp.size() - 1, 1);
}


/*
 * Multi-threaded version, using the number of threads specified
 * by setNumThreads().
 */
Result nearestPoints_DC_MT(vector<Point> &vp) {
	sortByX(vp, 0, vp.size() -1);
	return np_DC(vp, 0, vp.size() - 1, numThreads);
}
