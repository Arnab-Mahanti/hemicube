#include "Hemicube.h"
#include <cmath>
#include <iostream>
#include <omp.h>

using namespace std;
const double pi = atan(1.0)*4.0;

void Hemicube::precompute_view_factors()
{
    ///compute viewfactors on top and side face of hemicube
    double dx=2.0/x;
    double dy=2.0/y;
    double dz=1.0/z;

    double top_area=dx*dy;
    double side_area=dx*dz;

    ///Top Face
    for(size_t i=0; i<x; ++i)
        for(size_t j=0; j<y; ++j)
        {
            double xloc=(i-x/2.0+0.5)*dx;
            double yloc=(j-y/2.0+0.5)*dy;
            top_vf[i][j]= top_area/(pi*(xloc*xloc+yloc*yloc+1.0)*(xloc*xloc+yloc*yloc+1.0));
        }

    ///Side Face
    for(size_t i=0; i<x; ++i)
        for(size_t j=0; j<z; ++j)
        {
            double xloc=(i-x/2.0+0.5)*dx;
            double zloc=(j+0.5)*dz;
            side_vf[i][j]= side_area*zloc/(pi*(xloc*xloc+zloc*zloc+1.0)*(xloc*xloc+zloc*zloc+1.0));
        }
}

double** Hemicube::Get_Top_View_Factors()
{
    return top_vf;
}

double** Hemicube::Get_Side_View_Factors()
{
    return side_vf;
}




void Compute_HemiView_Factor::computeviewfactornew(size_t ID,point3D const& normal,point3D const& ncg,point3D const& ip1,point3D const& ip2,double const& area,std::unordered_map<size_t,double>& ViewFactor)
{
    unsigned char *top_value;
    unsigned char** side_values=new unsigned char*[4];

    double neardist = nearest[ID];

    std::chrono::_V2::steady_clock::time_point starter=std::chrono::steady_clock::now();

    renWin->GetRenderers()->InitTraversal();


    double near,far;

    vtkRenderer* current_renderer=renWin->GetRenderers()->GetNextItem();




    vtkCamera* activeCamera1=current_renderer->GetActiveCamera();



    /// Top Face

    activeCamera1->ParallelProjectionOff();
    activeCamera1->SetPosition(ncg[0],ncg[1],ncg[2]);
    activeCamera1->SetFocalPoint(normal[0]+ncg[0],normal[1]+ncg[1],normal[2]+ncg[2]);
    activeCamera1->SetViewUp(ip1[0],ip1[1],ip1[2]);
    activeCamera1->SetViewAngle(90.0);


    current_renderer->SetClippingRangeExpansion(0.1);
    current_renderer->ResetCameraClippingRange();


    activeCamera1->GetClippingRange(near,far);

    

    if(near>neardist*0.8)                           //Near Clipping Range to Nearest Face
            activeCamera1->SetClippingRange(neardist*0.8,far);
    


    current_renderer=renWin->GetRenderers()->GetNextItem();
    activeCamera1=current_renderer->GetActiveCamera();

    /// Side Face 1
    activeCamera1->ParallelProjectionOff();
    activeCamera1->SetPosition(ncg[0],ncg[1],ncg[2]);
    activeCamera1->SetFocalPoint(ip1[0]+ncg[0],ip1[1]+ncg[1],ip1[2]+ncg[2]);
    activeCamera1->SetViewUp(normal[0],normal[1],normal[2]);
    activeCamera1->SetViewAngle(90.0);
    current_renderer->SetClippingRangeExpansion(0.1);
    current_renderer->ResetCameraClippingRange();

    activeCamera1->GetClippingRange(near,far);

   if(near>neardist*0.8)                           //Near Clipping Range to Nearest Face
            activeCamera1->SetClippingRange(neardist*0.8,far);

    current_renderer=renWin->GetRenderers()->GetNextItem();
    activeCamera1=current_renderer->GetActiveCamera();

    /// Side Face 2
    activeCamera1->ParallelProjectionOff();
    activeCamera1->SetPosition(ncg[0],ncg[1],ncg[2]);
    activeCamera1->SetFocalPoint(ncg[0]-ip1[0],ncg[1]-ip1[1],ncg[2]-ip1[2]);
    activeCamera1->SetViewUp(normal[0],normal[1],normal[2]);
    activeCamera1->SetViewAngle(90.0);
    current_renderer->SetClippingRangeExpansion(0.1);
    current_renderer->ResetCameraClippingRange();

    activeCamera1->GetClippingRange(near,far);

    if(near>neardist*0.8)                           //Near Clipping Range to Nearest Face
            activeCamera1->SetClippingRange(neardist*0.8,far);
    current_renderer=renWin->GetRenderers()->GetNextItem();
    activeCamera1=current_renderer->GetActiveCamera();

    /// Side Face 3
    activeCamera1->ParallelProjectionOff();
    activeCamera1->SetPosition(ncg[0],ncg[1],ncg[2]);
    activeCamera1->SetFocalPoint(ip2[0]+ncg[0],ip2[1]+ncg[1],ip2[2]+ncg[2]);
    activeCamera1->SetViewUp(normal[0],normal[1],normal[2]);
    activeCamera1->SetViewAngle(90.0);
    current_renderer->SetClippingRangeExpansion(0.1);
    current_renderer->ResetCameraClippingRange();
    activeCamera1->GetClippingRange(near,far);

    if(near>neardist*0.8)                           //Near Clipping Range to Nearest Face
            activeCamera1->SetClippingRange(neardist*0.8,far);

    current_renderer=renWin->GetRenderers()->GetNextItem();
    activeCamera1=current_renderer->GetActiveCamera();

    /// Side Face 4
    activeCamera1->ParallelProjectionOff();
    activeCamera1->SetPosition(ncg[0],ncg[1],ncg[2]);
    activeCamera1->SetFocalPoint(-ip2[0]+ncg[0],-ip2[1]+ncg[1],-ip2[2]+ncg[2]);
    activeCamera1->SetViewUp(normal[0],normal[1],normal[2]);
    activeCamera1->SetViewAngle(90.0);
    current_renderer->SetClippingRangeExpansion(0.1);
    current_renderer->ResetCameraClippingRange();
    activeCamera1->GetClippingRange(near,far);

   if(near>neardist*0.8)                           //Near Clipping Range to Nearest Face
            activeCamera1->SetClippingRange(neardist*0.8,far);



    renWin->Render();
    top_value=renWin->GetPixelData(Resolution,Resolution,2*Resolution,2*Resolution,1);
    side_values[0]=renWin->GetPixelData(0,0,Resolution,Resolution,1);
    side_values[1]=renWin->GetPixelData(2*Resolution,0,3*Resolution,Resolution,1);
    side_values[2]=renWin->GetPixelData(2*Resolution,2*Resolution,3*Resolution,3*Resolution,1);
    side_values[3]=renWin->GetPixelData(0,2*Resolution,Resolution,3*Resolution,1);

    //starter=std::chrono::steady_clock::now();

    ///Parallelized Integration Top Face
    {
        TopFaceIntegrator sf(top_value,top_view,area,Resolution,elements.size());
        tbb::parallel_reduce(tbb::blocked_range2d<size_t,size_t>(0,Resolution,0,Resolution),sf);

        for(map<size_t,double>::iterator it=sf.my_sum.begin(); it!=sf.my_sum.end(); ++it)
            ViewFactor[it->first]+=it->second;
    }

    ///Parallelization Integration Side Faces
    {
        SideFaceIntegrator3 sf(side_values,side_view,area,Resolution,elements.size());
        tbb::parallel_reduce(tbb::blocked_range3d<size_t,size_t>(0,4,0,Resolution,Resolution/2,Resolution),sf);

        for(map<size_t,double>::iterator it=sf.my_sum.begin(); it!=sf.my_sum.end(); ++it)
            ViewFactor[it->first]+=it->second;
    }

    delete [] top_value;
    for(size_t i=0; i<4; ++i)
        delete [] side_values[i];
}

std::unordered_map<size_t,double> Compute_HemiView_Factor::computeviewfactor2(size_t ID,size_t level)
{
    element_subdivision* sub;
    point3D p1,p2,p3,p4;
    Element* e=elements[ID].Get_Element();
    double area=e->Get_Area();

    ///Identify if the element is tri or quad
    if(typeid(*e)==typeid(TriElement))
    {
        point3D p[3];
        for(size_t i=0; i<3; ++i)
        {
            size_t id=e->Get_New_Node_ID(i);
            p[i]=nodes[id];
        }
        sub=new element_subdivision(p[0],p[1],p[2],level);
    }

    if(typeid(*e)==typeid(QuadElement))
    {
        point3D p[4];
        for(size_t i=0; i<4; ++i)
        {
            size_t id=e->Get_New_Node_ID(i);
            p[i]=nodes[id];
        }
        sub=new element_subdivision(p[0],p[1],p[2],p[3],level);
    }

    vector<point3D> normals=sub->Get_Normals();
    vector<point3D> ncgs=sub->Get_Cgs();
    vector<point3D> ip1s=sub->Get_Ip1s();
    vector<point3D> ip2s=sub->Get_Ip2s();
    vector<double> areas=sub->Get_Areas();

    std::unordered_map<size_t,double> ViewFactor;

    //Scaling the normals and in-plane vector
    for(size_t i=0; i<areas.size(); ++i)
        computeviewfactornew(ID,normals[i],ncgs[i],ip1s[i],ip2s[i],areas[i],ViewFactor);

    ///Normalizing by the elemental area;
    for(auto it=ViewFactor.begin(); it!=ViewFactor.end(); ++it)
    {
        it->second/=area;

    }

    double vfsum=0;

    

    for(auto it=ViewFactor.begin(); it!=ViewFactor.end(); ++it)
    {
        if(it->first>0)
        {
            if(ID>=it->first)
            {
                double areai = elements[ID].Get_Element()->Get_Area();
                double areaj = elements[it->first-1].Get_Element()->Get_Area();

                for(int i = 0;i<ViewFactors[it->first-1].size();++i)
                {
                    if(ViewFactors[it->first-1][i].i==ID+1)
                        if(areai*it->second<areaj*ViewFactors[it->first-1][i].v)
                            it->second=areaj*ViewFactors[it->first-1][i].v/areai;
                }




            }
        }
    }

    return ViewFactor;
}


void Compute_HemiView_Factor::compute_view_factors(size_t ID)
{

    size_t n_sub_division=radcontrol->Get_Divisions();
    double error_criteria=radcontrol->Get_Error();
    std::unordered_map<size_t,double> ViewFactor1;

    if(error_criteria>0)        //Based on error criteria and maximum num of subdivisions
    {

        ///Computing without subdivision
        size_t level=0;
        bool idivide=false;
        size_t icount1=0,icount2=0;

        ViewFactor1=computeviewfactor2(ID,level);
        std::unordered_map<size_t,double> ViewFactor2;

        std::chrono::_V2::steady_clock::time_point starter=std::chrono::steady_clock::now();
        double error;

        do
        {
            ///Subdivding
            level++;
            ///level=0;
            ViewFactor2=computeviewfactor2(ID,level);

            ///Comparing the view factors
            error=0;
            for(auto it=ViewFactor2.begin(); it!=ViewFactor2.end(); it++)
            {
                auto itt = ViewFactor1.find(it->first);
                {
                    if(itt!=ViewFactor1.end())
                        error+=(itt->second-it->second)*(itt->second-it->second);

                    else
                        error+=it->second*it->second;

                    icount1++;
                }
            }

            for(auto it=ViewFactor1.begin(); it!=ViewFactor1.end(); it++)
            {
                auto itt = ViewFactor2.find(it->first);
                {
                    if(itt!=ViewFactor2.end())
                        error+=(itt->second-it->second)*(itt->second-it->second);
                    else
                        error+=it->second*it->second;

                    icount2++;
                }
            }

            error/=icount1*icount2;
            error=sqrt(error);

            ViewFactor1=ViewFactor2;

        }
        while(error>error_criteria&&level<n_sub_division);
    }
    else
    {
        ViewFactor1=computeviewfactor2(ID,n_sub_division);      //Based on subdivision alone
    }

    //Inserting into Vector

    for(auto it = ViewFactor1.begin();it!=ViewFactor1.end();++it)
    {
        chunk2 c;
        c.i= it->first;
        c.v = it->second;

        ViewFactors[ID].push_back(c);
    }



}
