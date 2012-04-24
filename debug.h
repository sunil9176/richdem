#ifndef _debug_included
#define _debug_included

#include <deque>
#include <cmath>

#define PRINT(ARR,PREC,WIDTH) std::cout<<std::setprecision(PREC)<<std::setw(WIDTH)<<ARR<<std::endl;

void print_edges(const float_2d &elevations, const std::deque<grid_cell> &low_edges, const std::deque<grid_cell> &high_edges);

template <class T> //TODO: Needs error checking for dimensions, et cetera
void ddiff(const array2d<T> &arr1, const array2d<T> &arr2, array2d<T> &result){
	diagnostic("Differencing the two arrays...\n");
	if(arr1.width()!=arr2.width() || arr1.height()!=arr2.height())
		diagnostic("failed! The arrays do not have the same dimensions!\n");
	result.copyprops(arr1);
	//Todo: It isn't guaranteed that subtraction of two elements will avoid the no_data value
	#pragma omp parallel for
	for(int x=0;x<arr1.width();x++)
	for(int y=0;y<arr2.height();y++)
		if(arr1(x,y)==arr1.no_data || arr2(x,y)==arr2.no_data)
			result(x,y)=arr1.no_data;
		else
			result(x,y)=fabs(arr1(x,y)-arr2(x,y));
	diagnostic("success!\n");
}

template <class T> //TODO: Needs error checking for dimensions, et cetera
void dadiff(const array2d<T> &arr1, const array2d<T> &arr2, array2d<T> &result){
	diagnostic("Differencing the two arrays...\n");
	if(arr1.width()!=arr2.width() || arr1.height()!=arr2.height())
		diagnostic("failed! The arrays do not have the same dimensions!\n");
	result.copyprops(arr1);
	//Todo: It isn't guaranteed that subtraction of two elements will avoid the no_data value
	#pragma omp parallel for
	for(int x=0;x<arr1.width();x++)
	for(int y=0;y<arr2.height();y++)
		if(arr1(x,y)==arr1.no_data || arr2(x,y)==arr2.no_data)
			result(x,y)=arr1.no_data;
		else{
			result(x,y)=fabs(arr1(x,y)-arr2(x,y));
			if(result(x,y)>2*M_PI)
				result(x,y)-=2*M_PI;
			else if (result(x,y)>M_PI)
				result(x,y)-=M_PI;
//			if(result(x,y)>0.17453292519943295769)
//				diagnostic_arg("%f-%f=%f\n",arr1(x,y),arr2(x,y),result(x,y));
		}
	diagnostic("success!\n");
}


template <class T>
void array2d<T>::print_block(std::ostream& out, int minx, int maxx, int miny, int maxy, int precision, std::streamsize swidth){
	out.setf(std::ios::fixed,std::ios::floatfield);
	out<<std::setprecision(precision);

	out<<std::setw(4)<<" "<<"\t";
	for(int x=((minx>0)?minx:0);x<=maxx && x<width();x++)
		out<<std::setw(swidth)<<x<<" ";
	out<<std::endl;

	for(int y=((miny>0)?miny:0);y<=maxy && y<height();y++){
		out<<std::setw(4)<<y<<"\t";
		for(int x=((minx>0)?minx:0);x<=maxx && x<width();x++)
			if(boost::numeric::ublas::matrix<T>::operator()(x,y)==no_data)
				out<<std::setw(swidth)<<"-"<<" ";
			else if(sizeof(T)==1)	//TODO: An ugly way of detecting chars
				out<<std::setw(swidth)<<(int)boost::numeric::ublas::matrix<T>::operator()(x,y)<<" ";
			else
				out<<std::setw(swidth)<<boost::numeric::ublas::matrix<T>::operator()(x,y)<<" ";
		out<<std::endl;
	}
}

#endif
