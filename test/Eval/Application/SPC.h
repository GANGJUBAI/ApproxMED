int Part1By1(int x);
int Part1By2(int x);
long int Part1By22(long int x);
int EncodeMorton2(int x, int y)
{
	return (Part1By1(y) << 1) + Part1By1(x);
}
long int EncodeMorton33(long int x, long int y, long int z)
{
        return (Part1By22(z) << 2) + (Part1By22(y) << 1) + Part1By22(x);
}

int EncodeMorton3(int x, int y, int z)
{
	return (Part1By2(z) << 2) + (Part1By2(y) << 1) + Part1By2(x);
}

// "Insert" a 0 bit after each of the 16 low bits of x
int Part1By1(int x)
{
	x &= 0x0000ffff;                  // x = ---- ---- ---- ---- fedc ba98 7654 3210
	x = (x ^ (x <<  8)) & 0x00ff00ff; // x = ---- ---- fedc ba98 ---- ---- 7654 3210
	x = (x ^ (x <<  4)) & 0x0f0f0f0f; // x = ---- fedc ---- ba98 ---- 7654 ---- 3210
	x = (x ^ (x <<  2)) & 0x33333333; // x = --fe --dc --ba --98 --76 --54 --32 --10
	x = (x ^ (x <<  1)) & 0x55555555; // x = -f-e -d-c -b-a -9-8 -7-6 -5-4 -3-2 -1-0
	return x;
}

long int Part1By22(long int x)
{
        x &= 0x000000000000ffff;                   // x = ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- fadc be98 7654 3210
        x = (x ^ (x <<  16)) & 0x00000000ff0000ff; // x = ---- ---- ---- ---- ---- ---- ---- ---- fadc be98 ---- ---- ---- ---- 7654 3210
        x = (x ^ (x <<  8))  & 0x000000f00f00f00f; // x = ---- ---- ---- ---- ---- ---- fadc ---- ---- be98 ---- ---- 7654 ---- ---- 3210
        x = (x ^ (x <<  4))  & 0x00000c30c30c30c3; // x = ---- ---- ---- ---- ---- fa-- --dc ---- be-- --98 ---- 76-- --54 ---- 32-- --10
        x = (x ^ (x <<  2))  & 0x0000249249249249; // x = ---- ---- ---- ---- --f- -a-- d--c --b- -e-- 9--8 --7- -6-- 5--4 --3- -2-- 1--0
        return x;
}

// "Insert" a 0 bits after each of the 12 low bits of x
/*int Part1By2(int x)
{
	x &= 0x00000fff;                  // x = ---- ---- ---- ---- ---- be98 7654 3210
	x = (x ^ (x <<  8)) & 0x000f00ff; // x = ---- ---- ---- be98 ---- ---- 7654 3210
	x = (x ^ (x <<  4)) & 0x000f0f0f; // x = ---- ---- ---- be98 ---- 7654 ---- 3210
	x = (x ^ (x <<  2)) & 0x00333333; // x = ---- ---- --be --98 --76 --54 --32 --10
	x = (x ^ (x <<  1)) & 0x00555555; // x = ---- ---- -b-e -9-8 -7-6 -5-4 -3-2 -1-0
	return x;
}*/
// "Insert" two 0 bits after each of the 10 low bits of x

int Part1By2(int x)
{
	x &= 0x000003ff;                  // x = ---- ---- ---- ---- ---- --98 7654 3210
	x = (x ^ (x << 16)) & 0x030000ff; // x = ---- --98 ---- ---- ---- ---- 7654 3210
	x = (x ^ (x <<  8)) & 0x0300f00f; // x = ---- --98 ---- ---- 7654 ---- ---- 3210
	x = (x ^ (x <<  4)) & 0x030c30c3; // x = ---- --98 ---- 76-- --54 ---- 32-- --10
	x = (x ^ (x <<  2)) & 0x09249249; // x = ---- 9--8 --7- -6-- 5--4 --3- -2-- 1--0
	return x;
}


// Inverse of Part1By1 - "delete" all odd-indexed bits
int Compact1By1(int x)
{
	x &= 0x55555555;                  // x = -f-e -d-c -b-a -9-8 -7-6 -5-4 -3-2 -1-0
	x = (x ^ (x >>  1)) & 0x33333333; // x = --fe --dc --ba --98 --76 --54 --32 --10
	x = (x ^ (x >>  2)) & 0x0f0f0f0f; // x = ---- fedc ---- ba98 ---- 7654 ---- 3210
	x = (x ^ (x >>  4)) & 0x00ff00ff; // x = ---- ---- fedc ba98 ---- ---- 7654 3210
	x = (x ^ (x >>  8)) & 0x0000ffff; // x = ---- ---- ---- ---- fedc ba98 7654 3210
	return x;
}

// Inverse of Part1By2 - "delete" all bits not at positions divisible by 3
int Compact1By2(int x)
{
	x &= 0x09249249;                  // x = ---- 9--8 --7- -6-- 5--4 --3- -2-- 1--0
	x = (x ^ (x >>  2)) & 0x030c30c3; // x = ---- --98 ---- 76-- --54 ---- 32-- --10
	x = (x ^ (x >>  4)) & 0x0300f00f; // x = ---- --98 ---- ---- 7654 ---- ---- 3210
	x = (x ^ (x >>  8)) & 0xff0000ff; // x = ---- --98 ---- ---- ---- ---- 7654 3210
	x = (x ^ (x >> 16)) & 0x000003ff; // x = ---- ---- ---- ---- ---- --98 7654 3210
	return x;
}

int DecodeMorton2X(int code)
{
	return Compact1By1(code >> 0);
}

int DecodeMorton2Y(int code)
{
	return Compact1By1(code >> 1);
}

int DecodeMorton3X(int code)
{
	return Compact1By2(code >> 0);
}

int DecodeMorton3Y(int code)
{
	return Compact1By2(code >> 1);
}

int DecodeMorton3Z(int code)
{
	return Compact1By2(code >> 2);
}

void rot(int n, int *x, int *y, int rx, int ry) {
	if (ry == 0) {
		if (rx == 1) {
			*x = n-1 - *x;
			*y = n-1 - *y;
		}

		//Swap x and y
		int t  = *x;
		*x = *y;
		*y = t;
	}
}
int xy2d (int n, int x, int y) {
	int rx, ry, s, d=0;
	for (s=n/2; s>0; s/=2) {
		rx = (x & s) > 0;
		ry = (y & s) > 0;
		d += s * s * ((3 * rx) ^ ry);
		rot(n, &x, &y, rx, ry);
	}
	return d;
}


int sgn(int x){
	return (x > 0) - (x < 0);

}

struct point{
	int x;
	int y;
	int z;
};

struct point * array;

void gilbert2d(int x, int y, int ax, int ay, int bx, int by)
{

	int w = abs(ax + ay);
	int h = abs(bx + by);
	int dax=sgn(ax);
	int day=sgn(ay);
	int dbx=sgn(bx);
	int dby=sgn(by);

	if (h == 1){
		// trivial row fill
		for(int i=0;i<w;i++){
			array->x=x;
			array->y=y;
			array++;
			x=x+dax;
			y=y+day;
		}
		return;
	}
	if (w == 1){
		// trivial  column fill
		for(int i=0;i<h;i++){
			array->x=x;
			array->y=y;
			array++;
			x=x+dbx;
			y=y+dby;
		}
		return;
	}

	int ax2=ax/2;
	int ay2=ay/2;
	int bx2=bx/2;
	int by2=by/2;



	int w2 = abs(ax2 + ay2);
	int h2 = abs(bx2 + by2);
	if( 2*w > 3*h){
		if (w2 % 2&&w > 2){
			// prefer even steps
			ax2=ax2 + dax;
			ay2=ay2 + day;
		}
		// long case: split in two parts only
		gilbert2d(x, y, ax2, ay2, bx, by);
		gilbert2d(x+ax2, y+ay2, ax-ax2, ay-ay2, bx, by);
	}
	else
	{
		if (h2 % 2&&h > 2){
			// prefer even steps
			bx2=bx2 + dbx;
			by2=by2 + dby;
		}
		//# standard case: one step up, one long horizontal, one step down
		gilbert2d(x, y, bx2, by2, ax2, ay2);
		gilbert2d(x+bx2, y+by2, ax, ay, bx-bx2, by-by2);
		gilbert2d(x+(ax-dax)+(bx2-dbx), y+(ay-day)+(by2-dby),-bx2, -by2, -(ax-ax2), -(ay-ay2));


	}



}

void gilbert3d(int x, int y,int z, int ax, int ay, int az,int bx, int by,int bz, int cx,int cy,int cz){
	int w = abs(ax + ay+az);
	int h = abs(bx + by+bz);
	int d=abs(cx+cy+cz);
	int dax=sgn(ax);
	int day=sgn(ay);
	int daz=sgn(az);

	int dbx=sgn(bx);
	int dby=sgn(by);
	int dbz=sgn(bz);

	int dcx=sgn(cx);
	int dcy=sgn(cy);
	int dcz=sgn(cz);

	if (h == 1&&d==1){
		// trivial row/colum fill
		for(int i=0;i<w;i++){
			array->x=x;
			array->y=y;
			array->z=z;
			array++;
			x=x+dax;
			y=y+day;
			z=z+daz;
		}
		return;
	}

	if (w == 1&&d==1){
		// trivial row/colum fill
		for(int i=0;i<h;i++){
			array->x=x;
			array->y=y;
			array->z=z;
			array++;
			x=x+dbx;
			y=y+dby;
			z=z+dbz;
		}
		return;
	}

	if (w == 1&&h==1){
		// trivial row/colum fill
		for(int i=0;i<d;i++){
			array->x=x;
			array->y=y;
			array->z=z;
			array++;
			x=x+dcx;
			y=y+dcy;
			z=z+dcz;
		}
		return;
	}

	int ax2=ax/2;
	int ay2=ay/2;
	int az2=az/2;

	int bx2=bx/2;
	int by2=by/2;
	int bz2=bz/2;

	int cx2=cx/2;
	int cy2=cy/2;
	int cz2=cz/2;

	int w2 = abs(ax2 + ay2+az2);
	int h2 = abs(bx2 + by2+bz2);
	int d2 = abs(cx2 + cy2+cz2);



	// prefer even steps
	if(w2%2&&w>2){
		ax2=ax2+dax;
		ay2=ay2+day;
		az2=az2+daz;
	}
	if(h2%2&&h>2){
		bx2=bx2+dbx;
		by2=by2+dby;
		bz2=bz2+dbz;
	}
	if(d2%2&&d>2){
		cx2=cx2+dcx;
		cy2=cy2+dcy;
		cz2=cz2+dcz;
	}

	//wide case, split in w only
	if( 2*w > 3*h&&2*w > 3*d){
		// long case: split in two parts only
		gilbert3d(x, y, z,
				ax2, ay2, az2,
				bx, by, bz,
				cx, cy, cz);

		gilbert3d(x+ax2, y+ay2, z+az2,
				ax-ax2, ay-ay2, az-az2,
				bx, by, bz,
				cx, cy, cz);
	}
	//do not split in d
	else if (3*h > 4*d)
	{

		gilbert3d(x, y, z,
				bx2, by2, bz2,
				cx, cy, cz,
				ax2, ay2, az2);

		gilbert3d(x+bx2, y+by2, z+bz2,
				ax, ay, az,
				bx-bx2, by-by2, bz-bz2,
				cx, cy, cz);

		gilbert3d(x+(ax-dax)+(bx2-dbx),
				y+(ay-day)+(by2-dby),
				z+(az-daz)+(bz2-dbz),
				-bx2, -by2, -bz2,
				cx, cy, cz,
				-(ax-ax2), -(ay-ay2), -(az-az2));



	}
	//do not split in h
	else if(3*d > 4*h)
	{


		gilbert3d(x, y, z,
				cx2, cy2, cz2,
				ax2, ay2, az2,
				bx, by, bz);

		gilbert3d(x+cx2, y+cy2, z+cz2,
				ax, ay, az,
				bx, by, bz,
				cx-cx2, cy-cy2, cz-cz2);

		gilbert3d(x+(ax-dax)+(cx2-dcx),
				y+(ay-day)+(cy2-dcy),
				z+(az-daz)+(cz2-dcz),
				-cx2, -cy2, -cz2,
				-(ax-ax2), -(ay-ay2), -(az-az2),
				bx, by, bz);



	}
	else

		// regular case, split in all w/h/d
	{

		gilbert3d(x, y, z,
				bx2, by2, bz2,
				cx2, cy2, cz2,
				ax2, ay2, az2);

		gilbert3d(x+bx2, y+by2, z+bz2,
				cx, cy, cz,
				ax2, ay2, az2,
				bx-bx2, by-by2, bz-bz2);

		gilbert3d(x+(bx2-dbx)+(cx-dcx),
				y+(by2-dby)+(cy-dcy),
				z+(bz2-dbz)+(cz-dcz),
				ax, ay, az,
				-bx2, -by2, -bz2,
				-(cx-cx2), -(cy-cy2), -(cz-cz2));

		gilbert3d(x+(ax-dax)+bx2+(cx-dcx),
				y+(ay-day)+by2+(cy-dcy),
				z+(az-daz)+bz2+(cz-dcz),
				-cx, -cy, -cz,
				-(ax-ax2), -(ay-ay2), -(az-az2),
				bx-bx2, by-by2, bz-bz2);

		gilbert3d(x+(ax-dax)+(bx2-dbx),
				y+(ay-day)+(by2-dby),
				z+(az-daz)+(bz2-dbz),
				-bx2, -by2, -bz2,
				cx2, cy2, cz2,
				-(ax-ax2), -(ay-ay2), -(az-az2));


	}

}
