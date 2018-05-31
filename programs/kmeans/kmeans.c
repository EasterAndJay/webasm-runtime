#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
typedef struct { double x, y; int group; } point_t, *point;
 
double randf(double m)
{
	return m * rand() / (RAND_MAX - 1.);
}
 
point gen_xy(int count, double radius)
{
	double ang, r;
	point p, pt = malloc(sizeof(point_t) * count);
 
	/* note: this is not a uniform 2-d distribution */
	for (p = pt + count; p-- > pt;) {
		ang = randf(2 * M_PI);
		r = randf(radius);
		p->x = r * cos(ang);
		p->y = r * sin(ang);
	}
 
	return pt;
}
 
inline double dist2(point a, point b)
{
	double x = a->x - b->x, y = a->y - b->y;
	return x*x + y*y;
}
 
inline int
nearest(point pt, point cent, int n_cluster, double *d2)
{
	int i, min_i;
	point c;
	double d, min_d;
 
#	define for_n for (c = cent, i = 0; i < n_cluster; i++, c++)
	for_n {
		min_d = HUGE_VAL;
		min_i = pt->group;
		for_n {
			if (min_d > (d = dist2(c, pt))) {
				min_d = d; min_i = i;
			}
		}
	}
	if (d2) *d2 = min_d;
	return min_i;
}
 
void kpp(point pts, int len, point cent, int n_cent)
{
#	define for_len for (j = 0, p = pts; j < len; j++, p++)
	int i, j;
	int n_cluster;
	double sum, *d = malloc(sizeof(double) * len);
 
	point p, c;
	cent[0] = pts[ rand() % len ];
	for (n_cluster = 1; n_cluster < n_cent; n_cluster++) {
		sum = 0;
		for_len {
			nearest(p, cent, n_cluster, d + j);
			sum += d[j];
		}
		sum = randf(sum);
		for_len {
			if ((sum -= d[j]) > 0) continue;
			cent[n_cluster] = pts[j];
			break;
		}
	}
	for_len p->group = nearest(p, cent, n_cluster, 0);
	free(d);
}
 
point lloyd(point pts, int len, int n_cluster)
{
	int i, j, min_i;
	int changed;
 
	point cent = malloc(sizeof(point_t) * n_cluster), p, c;
 
	/* assign init grouping randomly */
	//for_len p->group = j % n_cluster;
 
	/* or call k++ init */
	kpp(pts, len, cent, n_cluster);
 
	do {
		/* group element for centroids are used as counters */
		for_n { c->group = 0; c->x = c->y = 0; }
		for_len {
			c = cent + p->group;
			c->group++;
			c->x += p->x; c->y += p->y;
		}
		for_n { c->x /= c->group; c->y /= c->group; }
 
		changed = 0;
		/* find closest centroid of each point */
		for_len {
			min_i = nearest(p, cent, n_cluster, 0);
			if (min_i != p->group) {
				changed++;
				p->group = min_i;
			}
		}
	} while (changed > (len >> 10)); /* stop when 99.9% of points are good */
 
	for_n { c->group = i; }
 
	return cent;
}
 
void print_eps(point pts, int len, point cent, int n_cluster)
{
#	define W 400
#	define H 400
	int i, j;
	point p, c;
	double min_x, max_x, min_y, max_y, scale, cx, cy;
	double *colors = malloc(sizeof(double) * n_cluster * 3);
 
	for_n {
		colors[3*i + 0] = (3 * (i + 1) % 11)/11.;
		colors[3*i + 1] = (7 * i % 11)/11.;
		colors[3*i + 2] = (9 * i % 11)/11.;
	}
 
	max_x = max_y = -(min_x = min_y = HUGE_VAL);
	for_len {
		if (max_x < p->x) max_x = p->x;
		if (min_x > p->x) min_x = p->x;
		if (max_y < p->y) max_y = p->y;
		if (min_y > p->y) min_y = p->y;
	}
	scale = W / (max_x - min_x);
	if (scale > H / (max_y - min_y)) scale = H / (max_y - min_y);
	cx = (max_x + min_x) / 2;
	cy = (max_y + min_y) / 2;
 
	printf("%%!PS-Adobe-3.0\n%%%%BoundingBox: -5 -5 %d %d\n", W + 10, H + 10);
	printf( "/l {rlineto} def /m {rmoveto} def\n"
		"/c { .25 sub exch .25 sub exch .5 0 360 arc fill } def\n"
		"/s { moveto -2 0 m 2 2 l 2 -2 l -2 -2 l closepath "
		"	gsave 1 setgray fill grestore gsave 3 setlinewidth"
		" 1 setgray stroke grestore 0 setgray stroke }def\n"
	);
	for_n {
		printf("%g %g %g setrgbcolor\n",
			colors[3*i], colors[3*i + 1], colors[3*i + 2]);
		for_len {
			if (p->group != i) continue;
			printf("%.3f %.3f c\n",
				(p->x - cx) * scale + W / 2,
				(p->y - cy) * scale + H / 2);
		}
		printf("\n0 setgray %g %g s\n",
			(c->x - cx) * scale + W / 2,
			(c->y - cy) * scale + H / 2);
	}
	printf("\n%%%%EOF");
	free(colors);
#	undef for_n
#	undef for_len
}
 
#define PTS 100000
#define K 11
int main()
{
	int i;
	point v = gen_xy(PTS, 10);
	point c = lloyd(v, PTS, K);
	print_eps(v, PTS, c, K);
	// free(v); free(c);
	return 0;
}
D
Translation of: C
import std.stdio, std.math, std.random, std.typecons, std.algorithm;
 
// On Windows this uses the printf from the Microsoft C runtime,
// that doesn't handle real type and some of the C99 format
// specifiers, but it's faster for bulk printing.
extern(C) nothrow int printf(const char*, ...);
 
struct Point {
    immutable double x, y; // Or float.
    size_t cluster;
}
 
Point[] generatePoints(in size_t nPoints,
                       in double radius,
                       ref Xorshift rnd)
in {
    assert(nPoints > 0);
    assert(radius > 0);
} out(result) {
    assert(result.length == nPoints);
    foreach (const ref p; result) {
        assert(p.cluster == 0);
        assert(!p.x.isNaN && !p.y.isNaN);
    }
} body {
    Point[] points;
    points.reserve(nPoints);
 
    // This is not a uniform 2D distribution.
    foreach (immutable i; 0 .. nPoints) {
        immutable r = uniform(0.0, radius, rnd);
        immutable ang = uniform(0.0, 2 * PI, rnd);
        points ~= Point(r * ang.cos, r * ang.sin); // Sincos?
    }
 
    return points;
}
 
 
struct ClusterCenter {
    double x, y;
    void opAssign(in ref Point p) pure nothrow @nogc {
        this.x = p.x;
        this.y = p.y;
    }
}
 
 
const(ClusterCenter)[] lloyd(Point[] points,
                             in size_t nclusters,
                             ref Xorshift rnd)
in {
    assert(points.length >= nclusters);
    assert(nclusters > 0);
    foreach (const ref p; points)
        assert(!p.x.isNaN && !p.y.isNaN);
} out(result) {
    assert(result.length == nclusters);
    foreach (const ref cc; result)
        assert(!cc.x.isNaN && !cc.y.isNaN);
} body {
    /// Distance and index of the closest cluster center.
    static Tuple!(size_t, double)
    nearestClusterCenter(in ref Point point,
                         in ClusterCenter[] centers) pure nothrow @nogc
    in {
        assert(centers.length > 0);
    } out(result) {
        assert(result[0] < centers.length);
        immutable ClusterCenter c = centers[result[0]];
        immutable d = (c.x - point.x) ^^ 2  +  (c.y - point.y) ^^ 2;
        assert(feqrel(result[1], d) > 45); // Arbitrary.
    } body {
        static double sqrDistance2D(in ref ClusterCenter a,
                                    in ref Point b) pure nothrow @nogc{
            return (a.x - b.x) ^^ 2 + (a.y - b.y) ^^ 2;
        }
 
        size_t minIndex = point.cluster;
        double minDist = double.max;
 
        foreach (immutable i, const ref cc; centers) {
            immutable d = sqrDistance2D(cc, point);
            if (minDist > d) {
                minDist = d;
                minIndex = i;
            }
        }
 
        return tuple(minIndex, minDist);
    }
 
 
    static void kMeansPP(Point[] points,
                         ClusterCenter[] centers,
                         ref Xorshift rnd)
    in {
        assert(points.length >= centers.length);
        assert(centers.length > 0);
    } body {
        centers[0] = points[uniform(0, $, rnd)];
        auto d = new double[points.length];
 
        foreach (immutable i; 1 .. centers.length) {
            double sum = 0;
            foreach (immutable j, const ref p; points) {
                d[j] = nearestClusterCenter(p, centers[0 .. i])[1];
                sum += d[j];
            }
 
            sum = uniform(0.0, sum, rnd);
 
            foreach (immutable j, immutable dj; d) {
                sum -= dj;
                if (sum > 0)
                    continue;
                centers[i] = points[j];
                break;
            }
        }
 
        foreach (ref p; points)
            // Implicit cast of Hconst!ClusterCenter
            // to ClusterCenter[].
            p.cluster = nearestClusterCenter(p, centers)[0];
    }
 
 
    auto centers = new ClusterCenter[nclusters];
    kMeansPP(points, centers, rnd);
    auto clusterSizes = new size_t[centers.length];
 
    size_t changed;
    do {
        // Find clusters centroids.
        centers[] = ClusterCenter(0, 0);
        clusterSizes[] = 0;
 
        foreach (immutable i, const ref p; points)
            with (centers[p.cluster]) {
                clusterSizes[p.cluster]++;
                x += p.x;
                y += p.y;
            }
 
        foreach (immutable i, ref cc; centers) {
            cc.x /= clusterSizes[i];
            cc.y /= clusterSizes[i];
        }
 
        // Find closest centroid of each point.
        changed = 0;
        foreach (ref p; points) {
            immutable minI = nearestClusterCenter(p, centers)[0];
            if (minI != p.cluster) {
                changed++;
                p.cluster = minI;
            }
        }
    // Stop when 99.9% of points are good.
    } while (changed > (points.length >> 10));
 
    return centers;
}
 
 
void printEps(in Point[] points, in ClusterCenter[] centers,
              in size_t W = 400, in size_t H = 400) nothrow
in {
    assert(points.length >= centers.length);
    assert(centers.length > 0);
    assert(W > 0 && H > 0);
    foreach (const ref p; points)
        assert(!p.x.isNaN && !p.y.isNaN);
    foreach (const ref cc; centers)
        assert(!cc.x.isNaN && !cc.y.isNaN);
} body {
    auto findBoundingBox() nothrow @nogc {
        double min_x, max_x, min_y, max_y;
        max_x = max_y = -double.max;
        min_x = min_y = double.max;
 
        foreach (const ref p; points) {
            if (max_x < p.x) max_x = p.x;
            if (min_x > p.x) min_x = p.x;
            if (max_y < p.y) max_y = p.y;
            if (min_y > p.y) min_y = p.y;
        }
        assert(max_x > min_x && max_y > min_y);
 
        return tuple(min(W / (max_x - min_x), H / (max_y - min_y)),
                     (max_x + min_x) / 2, (max_y + min_y) / 2);
    }
    //immutable (scale, cx, cy) = findBoundingBox();
    immutable sc_cx_cy = findBoundingBox();
    immutable double scale = sc_cx_cy[0];
    immutable double cx = sc_cx_cy[1];
    immutable double cy = sc_cx_cy[2];
 
    static immutable struct Color { immutable double r, g, b; }
 
    immutable size_t k = centers.length;
    Color[] colors;
    colors.reserve(centers.length);
    foreach (immutable i; 0 .. centers.length)
        colors ~= Color((3 * (i + 1) % k) / double(k),
                        (7 * i % k) / double(k),
                        (9 * i % k) / double(k));
 
    printf("%%!PS-Adobe-3.0\n%%%%BoundingBox: -5 -5 %d %d\n",
           W + 10, H + 10);
 
    printf("/l {rlineto} def /m {rmoveto} def\n" ~
           "/c { .25 sub exch .25 sub exch .5 0 360 arc fill } def\n" ~
           "/s { moveto -2 0 m 2 2 l 2 -2 l -2 -2 l closepath " ~
           "   gsave 1 setgray fill grestore gsave 3 setlinewidth" ~
           " 1 setgray stroke grestore 0 setgray stroke }def\n");
 
    foreach (immutable i, const ref cc; centers) {
        printf("%g %g %g setrgbcolor\n", colors[i].tupleof);
 
        foreach (const ref p; points) {
            if (p.cluster != i)
                continue;
            printf("%.3f %.3f c\n",
                   (p.x - cx) * scale + W / 2,
                   (p.y - cy) * scale + H / 2);
        }
 
        printf("\n0 setgray %g %g s\n",
               (cc.x - cx) * scale + W / 2,
               (cc.y - cy) * scale + H / 2);
    }
 
    "\n%%%%EOF".printf;
}
 
 
void main() {
    enum size_t nPoints = 100_000;
    enum size_t nClusters = 11; // k.
    auto rnd = 1.Xorshift; // For speed and repeatability.
 
    auto points = generatePoints(nPoints, 10, rnd);
    const clusterCenters = lloyd(points, nClusters, rnd);
    printEps(points, clusterCenters);
}
