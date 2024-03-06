#ifndef HEMICUBE_H_INCLUDED
#define HEMICUBE_H_INCLUDED
#include<iostream>
#include<map>
#include<chrono>
#include "AnsystoVtk.h"
#include "vtkRendererCollection.h"
#include "vtkCollectionIterator.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"
#include "vtkUnsignedCharArray.h"

#include "element_subdivision.h"
#include <tbb/blocked_range2d.h>
#include <tbb/blocked_range3d.h>
#include <tbb/parallel_reduce.h>
#include <tbb/parallel_for.h>
#include <tbb/concurrent_hash_map.h>

using namespace std;

class Hemicube
{
	size_t x,y,z;                    ///no of cells in x and y and z directions
	double** top_vf, **side_vf;
	void precompute_view_factors();

public:
	Hemicube(size_t X):x(X),y(X),z(X/2)
	{
		top_vf=new double* [x];
		for(size_t i=0;i<x;++i)
			top_vf[i]=new double[y];
		side_vf=new double* [x];
		for(size_t i=0;i<x;++i)
			side_vf[i]=new double[z];
		precompute_view_factors();
	}

	~Hemicube()
	{
		for(size_t i=0;i<x;++i)
			delete[] top_vf[i];
		for(size_t i=0;i<x;++i)
			delete[] side_vf[i];
		delete[] top_vf;
		delete[] side_vf;
	}

	double** Get_Top_View_Factors();
	double** Get_Side_View_Factors();
};

class Compute_HemiView_Factor
{
    vtkSmartPointer<vtkRenderWindow> renWin;
    size_t Resolution;
    Hemicube h;
    std::vector<RadElement>& elements;
    std::vector<double>& nearest;
    //std::map<size_t,double>*  ViewFactors;
    std::vector<chunk2>* ViewFactors;

    vtkActorCollection *actors;
    vtkCamera  *activeCamera;
    double** top_view;
    double** side_view;
    void computeviewfactornew(size_t,point3D const&,point3D const&,point3D const&,point3D const&,double const&,std::unordered_map<size_t,double>&);
    std::unordered_map<size_t,double> computeviewfactor2(size_t,size_t);


public:
    Compute_HemiView_Factor(vtkSmartPointer<vtkRenderWindow> RENWIN,size_t RESOLUTION,std::vector<RadElement>& ELEMENTS,std::vector<double>& NEAR,std::vector<chunk2>* VF):renWin(RENWIN),Resolution(RESOLUTION), h(RESOLUTION)
    ,elements(ELEMENTS),nearest(NEAR),ViewFactors(VF)
    {
        actors= renWin->GetRenderers()->GetFirstRenderer()->GetActors();
        activeCamera = renWin->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
        top_view= h.Get_Top_View_Factors();
        side_view=h.Get_Side_View_Factors();
     }

    Compute_HemiView_Factor(size_t RESOLUTION,std::vector<RadElement>& ELEMENTS,std::vector<double>& NEAR,std::vector<chunk2>* VF):Resolution(RESOLUTION), h(RESOLUTION),elements(ELEMENTS),nearest(NEAR),ViewFactors(VF)
    {
        top_view= h.Get_Top_View_Factors();
        side_view=h.Get_Side_View_Factors();
    }

    void Set_RenWin(vtkSmartPointer<vtkRenderWindow> RENWIN)
    {
        renWin=RENWIN;
        actors= renWin->GetRenderers()->GetFirstRenderer()->GetActors();
        activeCamera = renWin->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
    }

    void compute_view_factors(size_t);
};

class TopFaceIntegrator
{
	unsigned char* my_value;
	double** my_a;
	double area;
	size_t Resolution;
	size_t total_elements;

public:
	map<size_t,double> my_sum;
	void operator()( const tbb::blocked_range2d<size_t>& r )
	{
		unsigned char* value=my_value;
		double **a = my_a;

		for( size_t i=r.rows().begin(); i!=r.rows().end(); ++i )
			for( size_t j=r.cols().begin(); j!=r.cols().end(); ++j )
			{
				size_t d0=value[3*(i+j*(Resolution+1))];
				size_t d1=value[3*(i+j*(Resolution+1))+1];
				size_t d2=value[3*(i+j*(Resolution+1))+2];

				///Converting color back to ID
				size_t id = (d0*256*256+d1*256+d2);

				///Testing Code for reverse side compatibility
                if(id<=total_elements)
					my_sum[id]+=a[i][j]*area;
			}
	}
	TopFaceIntegrator( TopFaceIntegrator& x, tbb::split ) : my_value(x.my_value),my_a(x.my_a),area(x.area),Resolution(x.Resolution),total_elements(x.total_elements),my_sum()
	{}

	void join( const TopFaceIntegrator& y )
	{
		for(map<size_t,double>::const_iterator it=y.my_sum.begin();it!=y.my_sum.end();++it)
			my_sum[it->first]+=it->second;
	}

	TopFaceIntegrator(unsigned char *value,double **a, double AREA,size_t RES,size_t TOT):my_value(value),my_a(a),area(AREA),Resolution(RES),total_elements(TOT),my_sum()
	{}
};

class SideFaceIntegrator3
{
	unsigned char** my_value;
	double** my_a;
	double area;
	size_t Resolution;
	size_t total_elements;

public:
	map<size_t,double> my_sum;
	void operator()( const tbb::blocked_range3d<size_t>& r )
	{
		unsigned char** value=my_value;
		double **a = my_a;

		for( size_t k=r.pages().begin(); k!=r.pages().end(); ++k )
			for( size_t i=r.rows().begin(); i!=r.rows().end(); ++i )
				for( size_t j=r.cols().begin(); j!=r.cols().end(); ++j )
				{
					size_t d0=value[k][3*(i+j*(Resolution+1))];
					size_t d1=value[k][3*(i+j*(Resolution+1))+1];
					size_t d2=value[k][3*(i+j*(Resolution+1))+2];

					///Converting color back to ID
					size_t id = (d0*256*256+d1*256+d2);

					///Test Code for reverse side compatibility
                    if(id<=total_elements)
						my_sum[id]+=a[i][j-Resolution/2]*area;
				}
	}
	SideFaceIntegrator3( SideFaceIntegrator3& x, tbb::split ) : my_value(x.my_value),my_a(x.my_a),area(x.area),Resolution(x.Resolution),total_elements(x.total_elements),my_sum()
	{}

	void join( const SideFaceIntegrator3& y )
	{
		for(map<size_t,double>::const_iterator it=y.my_sum.begin();it!=y.my_sum.end();++it)
			my_sum[it->first]+=it->second;
	}

	SideFaceIntegrator3(unsigned char **value,double **a, double AREA,size_t RES,size_t TOT):my_value(value),my_a(a),area(AREA),Resolution(RES),total_elements(TOT),my_sum()
	{}
};


#endif // HEMICUBE_H_INCLUDED

