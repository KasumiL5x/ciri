#ifndef __plane__
#define __plane__

#include <vector>
#include <cc/Vec3.hpp>
#include <cc/Vec4.hpp>

struct Plane {
	cc::Vec3f normal;
	float constant;

	Plane()
		: normal(cc::Vec3f::up()), constant(0.0f) {
	}

	Plane( const cc::Vec3f& inNormal, const float inConstant )
		: normal(inNormal), constant(inConstant) {
	}

	float unsignedDistance( const cc::Vec3f& point ) const {
		return fabsf(normal.x*point.x + normal.y*point.y + normal.z*point.z + constant);
	}

	float signedDistance( const cc::Vec3f& point ) const {
		return (normal.x * point.x + normal.y * point.y + normal.z * point.z + constant);
	}

	static Plane constructFromPointNormal( const cc::Vec3f& point, const cc::Vec3f& normal ) {
		Plane result;
		const cc::Vec3f normalizedNormal = normal.normalized();
		result.normal = normalizedNormal;
		result.constant = -point.dot(normalizedNormal);
		return result;
	}

	static bool fitPointsToPlane( const std::vector<cc::Vec3f>& points, float& residualError, Plane& outPlane );
	static bool fitPointsToPlane( const std::vector<cc::Vec4f>& points, cc::Vec3f& basis1, cc::Vec3f& basis2, float& normalEigenValue, float& residualError, Plane& outPlane );

};

static void findScatterMatrix( const std::vector<cc::Vec4f>& points, const cc::Vec3f& centroid, float scatterMatrix[3][3], int order[3] ) {
	int i,TempI;
	float TempD;
	for( i=0; i<3; i++ ) {
		scatterMatrix[i][0]=scatterMatrix[i][1]=scatterMatrix[i][2]=0;
	}
	for( unsigned int i = 0; i < static_cast<unsigned int>(points.size()); i++ ) {
		const cc::Vec4f& P = points[i];
		cc::Vec3f d = cc::Vec3f(P.x, P.y, P.z) - centroid;
		float Weight = P.w;
		scatterMatrix[0][0] += d.x*d.x*Weight;
		scatterMatrix[0][1] += d.x*d.y*Weight;
		scatterMatrix[0][2] += d.x*d.z*Weight;
		scatterMatrix[1][1] += d.y*d.y*Weight;
		scatterMatrix[1][2] += d.y*d.z*Weight;
		scatterMatrix[2][2] += d.z*d.z*Weight;
	}
	scatterMatrix[1][0]=scatterMatrix[0][1];
	scatterMatrix[2][0]=scatterMatrix[0][2];
	scatterMatrix[2][1]=scatterMatrix[1][2];
	order[0]=0;
	order[1]=1;
	order[2]=2;
	if( scatterMatrix[0][0] > scatterMatrix[1][1] ) {
		TempD=scatterMatrix[0][0];
		scatterMatrix[0][0]=scatterMatrix[1][1];
		scatterMatrix[1][1]=TempD;
		TempD=scatterMatrix[0][2];
		scatterMatrix[0][2]=scatterMatrix[2][0]=scatterMatrix[1][2];
		scatterMatrix[1][2]=scatterMatrix[2][1]=TempD;
		TempI=order[0];
		order[0]=order[1];
		order[1]=TempI;
	}
	if( scatterMatrix[1][1] > scatterMatrix[2][2] ) {
		TempD=scatterMatrix[1][1];
		scatterMatrix[1][1]=scatterMatrix[2][2];
		scatterMatrix[2][2]=TempD;
		TempD=scatterMatrix[0][1];
		scatterMatrix[0][1]=scatterMatrix[1][0]=scatterMatrix[0][2];
		scatterMatrix[0][2]=scatterMatrix[2][0]=TempD;
		TempI=order[1];
		order[1]=order[2];
		order[2]=TempI;
	}
	if( scatterMatrix[0][0] > scatterMatrix[1][1] ) {
		TempD=scatterMatrix[0][0];
		scatterMatrix[0][0]=scatterMatrix[1][1];
		scatterMatrix[1][1]=TempD;
		TempD=scatterMatrix[0][2];
		scatterMatrix[0][2]=scatterMatrix[2][0]=scatterMatrix[1][2];
		scatterMatrix[1][2]=scatterMatrix[2][1]=TempD;
		TempI=order[0];
		order[0]=order[1];
		order[1]=TempI;
	}
}

static void tred2( float a[3][3], float d[3], float e[3] ) {
  int        l,k,i,j;
  float    scale,hh,h,g,f;

    for(i=3;i>=2;i--)
    {
    l=i-1;
    h=scale=0.0;
    if(l>1)
        {
        for(k=1;k<=l;k++)
            scale+=fabs(a[i-1][k-1]);
        if(scale==0.0)        /* skip transformation */
            e[i-1]=a[i-1][l-1];
        else
            {
            for(k=1;k<=l;k++)
                {
                a[i-1][k-1]/=scale;    /* use scaled a's for transformation. */
                h+=a[i-1][k-1]*a[i-1][k-1];    /* form sigma in h. */
                }
            f=a[i-1][l-1];
            g=f>0? -sqrt(h):sqrt(h);
            e[i-1]=scale*g;
            h-=f*g;    /* now h is equation (11.2.4) */
            a[i-1][l-1]=f-g;    /* store u in the ith row of a. */
            f=0.0;
            for(j=1;j<=l;j++)
                {
                a[j-1][i-1]=a[i-1][j-1]/h; /* store u/H in ith column of a. */
                g=0.0;    /* form an element of A.u in g */
                for(k=1;k<=j;k++)
                    g+=a[j-1][k-1]*a[i-1][k-1];
                for(k=j+1;k<=l;k++)
                    g+=a[k-1][j-1]*a[i-1][k-1];
                e[j-1]=g/h; /* form element of p in temorarliy unused element of e. */
                f+=e[j-1]*a[i-1][j-1];
                }
            hh=f/(h+h);    /* form K, equation (11.2.11) */
            for(j=1;j<=l;j++) /* form q and store in e overwriting p. */
                {
                f=a[i-1][j-1]; /* Note that e[l]=e[i-1] survives */
                e[j-1]=g=e[j-1]-hh*f;
                for(k=1;k<=j;k++) /* reduce a, equation (11.2.13) */
                    a[j-1][k-1]-=(f*e[k-1]+g*a[i-1][k-1]);
                }
            }
        }
    else
        e[i-1]=a[i-1][l-1];
    d[i-1]=h;
    }


  /*
  **    For computing eigenvector.
  */
  d[0]=0.0;
  e[0]=0.0;

  for(i=1;i<=3;i++)/* begin accumualting of transfomation matrices */
    {
    l=i-1;
    if(d[i-1]) /* this block skipped when i=1 */
        {
        for(j=1;j<=l;j++)
            {
            g=0.0;
            for(k=1;k<=l;k++) /* use u and u/H stored in a to form P.Q */
                g+=a[i-1][k-1]*a[k-1][j-1];
            for(k=1;k<=l;k++)
                a[k-1][j-1]-=g*a[k-1][i-1];
            }
        }
    d[i-1]=a[i-1][i-1];
    a[i-1][i-1]=1.0; /* reset row and column of a to identity matrix for next iteration */
    for(j=1;j<=l;j++)
        a[j-1][i-1]=a[i-1][j-1]=0.0;
    }
}

#define    SIGN(a,b)    ((b)<0? -fabs(a):fabs(a))
static void tqli( float d[3], float e[3], float z[3][3] ) {
  int        m,l,iter,i,k;
  float    s,r,p,g,f,dd,c,b;

  for(i=2;i<=3;i++)
    e[i-2]=e[i-1];    /* convenient to renumber the elements of e */
  e[2]=0.0;
  for(l=1;l<=3;l++)
    {
    iter=0;
    do
        {
        for(m=l;m<=2;m++)
            {
            /*
            **    Look for a single small subdiagonal element
            **    to split the matrix.
            */
            dd=fabs(d[m-1])+fabs(d[m]);
            if(fabs(e[m-1])+dd == dd)
                break;
            }
        if(m!=l)
            {
            if(iter++ == 30)
                {
                printf("\nToo many iterations in TQLI");
                }
            g=(d[l]-d[l-1])/(2.0f*e[l-1]); /* form shift */
            r=sqrt((g*g)+1.0f);
            g=d[m-1]-d[l-1]+e[l-1]/(g+SIGN(r,g)); /* this is dm-ks */
            s=c=1.0;
            p=0.0;
            for(i=m-1;i>=l;i--)
                {
                /*
                **    A plane rotation as in the original
                **    QL, followed by Givens rotations to
                **    restore tridiagonal form.
                */
                f=s*e[i-1];
                b=c*e[i-1];
                if(fabs(f) >= fabs(g))
                    {
                    c=g/f;
                    r=sqrt((c*c)+1.0f);
                    e[i]=f*r;
                    c*=(s=1.0f/r);
                    }
                else
                    {
                    s=f/g;
                    r=sqrt((s*s)+1.0f);
                    e[i]=g*r;
                    s*=(c=1.0f/r);
                    }
                g=d[i]-p;
                r=(d[i-1]-g)*s+2.0f*c*b;
                p=s*r;
                d[i]=g+p;
                g=c*r-b;
                for(k=1;k<=3;k++)
                    {
                    /*
                    **    Form eigenvectors
                    */
                    f=z[k-1][i];
                    z[k-1][i]=s*z[k-1][i-1]+c*f;
                    z[k-1][i-1]=c*z[k-1][i-1]-s*f;
                    }
                }
            d[l-1]=d[l-1]-p;
            e[l-1]=g;
            e[m-1]=0.0f;
            }
        }while(m != l);
    }
}

static void setVec3Length( cc::Vec3f& vec, float newLength ) {
	const float length = vec.magnitude();
	if( length != 0.0f ) {
		const float factor = newLength / length;
		vec.x *= factor;
		vec.y *= factor;
		vec.z *= factor;
	}
}

static bool isVec3Valid( const cc::Vec3f& vec ) {
	return (vec.x==vec.x) && (vec.y==vec.y) && (vec.z==vec.z);
}

#endif /* __plane__ */