// Size of lookup table
#define MTHETA 2160		// DIVT*(azimuth or horizontal view angle = 360)
#define MPHI 605		// DIVP*(elevational or vertical view angle = 121)
#define DIVT 6
#define DIVP 5
// Parameters of omnidirectional vision system
#define X_MAX 20		// Mirror radius
#define Y_MAX 12.96		// Height of mirror
//#define Y_MAX 16.00		// Height of mirror
#define DFOCOS 82.915		// Distance between camera focus and mirror focus
//#define FCAMERA 12		// Focal distance of the camera//ORIGINAL
#define FCAMERA 12		// Focal distance of the camera //TEST

//#define CCD_VERT 4.84		// Vertical size of CCD
//#define CCD_HORZ 6.45		// Horizontal size of CCD
		/* Hyperbole function according Svoboda with a=30 e b=15 */
//#define FUNCAO f_mirror
//#define DFUNC df_mirror
#define ACU   0.000001
#define MAXCOLS 640
#define MAXROWS 480
// Lookup Tables
		int lookup_t[MPHI*MTHETA];
int lookup_ang[MAXCOLS*MAXROWS];


float f_mirror(float x) 
{ 
	return((sqrt(1374.97*(1 + (x*x)/343.74))-41.457));
}

float df_mirror(float x) 
{ 
	return(-x*(144176054272.0/(sqrt(0.1786295048423e25 + 0.5196645861473e22*x*x)))); 
}


void faux1(float ang, float x, float *fn, float *df) 
{ 
	*fn = f_mirror(x) - ang*x;	
	*df = df_mirror(x) - ang; 
}


float rtsafe(float ang, float x1, float x2, float xacc)
{
	int j;
	float df,dx,dxold,f,fh,fl;
	float temp,xh,xl,rts;
	faux1(ang,x1,&fl,&df);
	faux1(ang,x2,&fh,&df);
	if ((fl > 0.0 && fh > 0.0) || (fl < 0.0 && fh < 0.0))
	{
		printf("\nRoot must be bracketed in rtsafe\n");
		exit(1);
	}
	if (fl == 0.0) return x1;
	if (fh == 0.0) return x2;
	if (fl < 0.0)
	{
		xl=x1;
		xh=x2;
	}
	else
	{
		xh=x1;
		xl=x2;
	}
	rts=0.5*(x1+x2);
	dxold=fabs(x2-x1);
	dx=dxold;
	faux1(ang,rts,&f,&df);
	for (j=1;j<=100;j++)
	{
		if ((((rts-xh)*df-f)*((rts-xl)*df-f) >= 0.0) || (fabs(2.0*f) > fabs(dxold*df)))
		{
			dxold=dx;
			dx=0.5*(xh-xl);
			rts=xl+dx;
			if (xl==rts) return rts;
		}
		else
		{
			dxold = dx;
			dx=f/df;
			temp=rts;
			rts -= dx;
			if (temp==rts) return rts;
		}
		if (fabs(dx) < xacc) return rts;
		faux1(ang,rts,&f,&df);
		if (f<0.0)
			xl=rts;
		else
			xh=rts;
	}
	printf("\nMaximum number of iterations exceeded in rtsafe\n");
	exit(1);
	return 0.0;
}

float root(float ang) 
{ 
	return rtsafe(ang, 0, X_MAX, ACU); 
} 



void compute_lookuptables(int * lookup_ang, int *lookup_t, int centerx_omni, int centery_omni)
{
	int width_omni = MAXCOLS;	// width (pixels)
	int height_omni = MAXROWS;	// height (pixels)
//320 240
//	int centerx_omni = 320;//333;
//	int centery_omni = 240;//250;
	int i, j, xu, yu;
	float beta, phi, x, Pim;
	double xud, yud; 
//  cout<< "Computing Lookup Tables... Please wait..." << endl;
	printf("Computing Lookup Tables... Please wait...\n");
	for(i=0;i<MPHI;i++)
		for(j=0;j<MTHETA;j++)
			lookup_t[i*MTHETA +j] = -1;
	for(i=0;i<width_omni;i++)
		for(j=0;j<height_omni;j++)
			lookup_ang[j*width_omni +i] = -1;
	for (i=0;i<MPHI;i++)
	{
		for(j=0;j<MTHETA;j++)
		{
			//calculo do angulo incidente no espelho que formou o pixel
			phi = 3.1415926*(MPHI - i - 90*DIVP)/(180*DIVP);
			// tangente do angulo
			beta = tan(phi);
			//calculo da coordenada x do espelho
			x = root(beta);
			if (x > X_MAX)
				printf("Erro no calculo de x");
			if (x*beta > Y_MAX)
				printf("Erro no calculo de y");	
			//calculo da distancia radial em pixel do pixel que equivale ao angulo phi
			Pim = (x*(centery_omni)*((DFOCOS)+Y_MAX))/(X_MAX*((DFOCOS)+(x*beta)));
			//escrevendo as coordenadas da imagem retangular
			xud= -Pim*(cos((6.28*j)/(MTHETA-1)));
			yud= -Pim*(sin((6.28*j)/(MTHETA-1))); 
			xud=rint(xud);
			yud=rint(yud);
			xu = (int)xud + (int)(centerx_omni);
			yu = (int)yud + (int)(centery_omni);
			if(yu < 0)
				yu = 0;

			if(yu >= height_omni)
				yu = height_omni-1;
			if(xu < 0)
				xu = 0;
			if(xu >= width_omni)
				xu = width_omni-1;
			lookup_t[(i*MTHETA)+j]=(yu*width_omni)+xu;
			lookup_ang[(yu*width_omni)+xu] = (i*MTHETA)+j;
		}
	}
//	cout<< "Lookup Tables done." << endl;
	printf("Lookup Tables done.\n");
}

void compute_panoramica_colour(IplImage *src, IplImage *dst)
{
int temp=0;
	uchar *colorpixels=&((uchar*)(src->imageData+src->widthStep*0))[0*3];
//        uchar* positions=&((uchar*)(src->imageData+src->widthStep*0))[0*3];

	uchar *colorpixeld=&((uchar*)(dst->imageData+dst->widthStep*0))[0*3];
//        uchar* positiond=&((uchar*)(dst->imageData+dst->widthStep*0))[0*3];

	int largs=src->widthStep;
	int largd=dst->widthStep;
	for(int pos_y=0;pos_y<dst->height;pos_y++){
		for(int pos_x=0;pos_x<dst->width;pos_x++){
//pixeld[pos_y*larg+pos_x]=pixels[lookup_t[(int)((pos_y*MPHI)/dst->height)*MTHETA + (int)((pos_x*MTHETA)/dst->width)]];

                    //colorpixels[lookup_t[(int)((pos_y*MPHI)/dst->height)*MTHETA + (int)((pos_x*MTHETA)/dst->width)]*3]=255;
                    //colorpixeld=positiond+(pos_y*largd+pos_x*3);
//image is mirrored, change pos_x to correct it: in colorpixels, pos_x = width - pos_x
colorpixeld[pos_y*largd+pos_x*3]=colorpixels[lookup_t[(int)((pos_y*MPHI)/dst->height)*MTHETA + (int)(((dst->width-pos_x)*MTHETA)/dst->width)]*3];

colorpixeld[(pos_y*largd+pos_x*3)+1]=colorpixels[(lookup_t[(int)((pos_y*MPHI)/dst->height)*MTHETA + (int)(((dst->width-pos_x)*MTHETA)/dst->width)]*3)+1];

colorpixeld[(pos_y*largd+pos_x*3)+2]=colorpixels[(lookup_t[(int)((pos_y*MPHI)/dst->height)*MTHETA + (int)(((dst->width-pos_x)*MTHETA)/dst->width)]*3)+2];
//                    colorpixeld[1]=colorpixels[1];
//                    colorpixeld[2]=colorpixels[2];
//previous
//pixeld[pos_y*larg+pos_x]=pixels[lookup_t[(int)((pos_y*MPHI)/dst->height)*MTHETA + (int)((pos_x*MTHETA)/dst->width)]];



      //pixeld[pos_y*larg+pos_x]=pixels[(src->height-pos_y)*larg+pos_x];


//pixeld[pos_y*larg+pos_x*2]=pixels[lookup_t[(int)((pos_y*MPHI)/dst->height)*MTHETA + 2*(int)((pos_x*MTHETA)/dst->width)]];
//pixeld[pos_y*larg+pos_x*3]=pixels[lookup_t[(int)((pos_y*MPHI)/dst->height)*MTHETA + 3*(int)((pos_x*MTHETA)/dst->width)]];
		}
	}
}

void compute_perspective_colour( IplImage *src, IplImage *per, float phi_z, float theta_z, float f)
{
	//float phi_z =-0.1;		// elevational angle (vertical) in rad (from 0.1 to 1.3 rad)
	//float theta_z = 3.14;//-3.141592109*20/180;	// azimuth angle (horizontal) in rad (0 to 2pi)
	//float f = 170;
	//if (f<170) f=170;//dont know why it abbends if less ...

	float theta, phi, ctheta_z, stheta_z, cphi_z, sphi_z;
	// Unwarping - perspective image
	// theta_z is the azimuth (horizontal) angle of the perspective plane
	// phi_z is the elevational angle of the perspective plane
	// f is the focus distance measured in pixel of the perspective plane

	sphi_z = sin(phi_z);
	cphi_z = cos(phi_z);
	stheta_z = sin(theta_z);
	ctheta_z = cos(theta_z);
	uchar *pixels=&((uchar*)(src->imageData+src->widthStep*0))[0];
	uchar *pixeld=&((uchar*)(per->imageData+per->widthStep*0))[0];
	int largs=src->widthStep;
	int largd=per->widthStep;
	for(int pos_y=1;pos_y<per->height;pos_y++){
		for(int pos_x=1;pos_x<per->width;pos_x++){
			theta = 180*atan2( ((f*cphi_z - ((int)(-pos_y+per->height/2))*sphi_z)*stheta_z - ((int)(pos_x-per->width/2))*ctheta_z),((f*cphi_z - ((int)(-pos_y+per->height/2))*sphi_z)*ctheta_z + ((int)(pos_x-per->width/2))*stheta_z) )/3.14;
			phi = 180*atan2( (f*sphi_z + ((int)(-pos_y+per->height/2))*cphi_z),f*cphi_z )/3.14;
			if (theta < 0) theta = theta + 360;
 			phi = phi + 90;
//			*(w_ptr) = *(r_ptr + lookup_t[(((int)(MPHI-DIVP*phi))*MTHETA)+(int)(DIVT*theta)]);
			if ( (((((int)(MPHI-DIVP*phi))*MTHETA)+(int)(DIVT*theta))< (MTHETA*MPHI)) && (((((int)(MPHI-DIVP*phi))*MTHETA)+(int)(DIVT*theta))>0) )
			{
  			 if ( (lookup_t[(((int)(MPHI-DIVP*phi))*MTHETA)+(int)(DIVT*theta)] < 640*480) && 
						 (lookup_t[(((int)(MPHI-DIVP*phi))*MTHETA)+(int)(DIVT*theta)] >0 ))
			//if( lookup_t[(((int)(MPHI-DIVP*phi))*MTHETA)+(int)(DIVT*theta)] < (640*480) )
			 {

pixeld[pos_y*largd+pos_x*3]=pixels[lookup_t[(((int)(MPHI-DIVP*phi))*MTHETA)+(int)(DIVT*theta)]*3];
pixeld[(pos_y*largd+pos_x*3)+1]=pixels[(lookup_t[(((int)(MPHI-DIVP*phi))*MTHETA)+(int)(DIVT*theta)]*3)+1];
pixeld[(pos_y*largd+pos_x*3)+2]=pixels[(lookup_t[(((int)(MPHI-DIVP*phi))*MTHETA)+(int)(DIVT*theta)]*3)+2];



				//printf("%d\n",pixels[lookup_t[(((int)(MPHI-DIVP*phi))*MTHETA)+(int)(DIVT*theta)]]);
				//pixeld[pos_y*largd+pos_x]=255;
			 }
			 else {  pixeld[pos_y*largd+pos_x*3]=255;
                                 pixeld[(pos_y*largd+pos_x*3)+1]=255;
                                 pixeld[(pos_y*largd+pos_x*3)+2]=255;
                          }
			}
			else     {
                                 //printf("OVERFLOW\n");
                                 pixeld[pos_y*largd+pos_x*3]=0;
                                 pixeld[(pos_y*largd+pos_x*3)+1]=0;
                                 pixeld[(pos_y*largd+pos_x*3)+2]=0;
                        }
		}
	}
}
