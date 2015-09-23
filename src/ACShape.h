/*
 * Shape.h
 * 形状，一个抽象类。目前主要考虑了2D面，相当于投影。
 * 最小的部分，包括直线（2点）、三角（3点）、多边形（3+点），曲线（一级抽象），
 * 包括闭合形状，三角形，多边形，圆形，
 * 
 * 要考虑包含3D模式的形状，三角锥面，长方体面，多边体面，弧形球面。
 *
 *  Created on: 2015年9月7日
 *      Author: qiang.liu
 */


//#include "Shadow.h
#include <ACEnv.h>
#include <ACShapeFeature.h>

#include "ACCamera.h"
#include "ACShadow.h"
//#include "opencv2/opencv.hpp"

#ifndef SHAPE_H_
#define SHAPE_H_

namespace ac {

class Shape  {
public:
	Shape();
	virtual ~Shape();

	void sayHello();
	//CString* toString();

	//***********************************************************
	/*
	 * 在指定面上的投影，为了加速，临时变量。
	 * BTW: 明显线条，应该成为特征。
	 */
	Shadow* shadow;

	/*
	 * 获得在指定面上的投影。
	 */
	 Shadow* getAShadow(Camera* carmea);
	 Shadow* getAShadow();

	//***********************************************************
	/*
	 * 形状特征。
	 */
	ShapeFeature* feature;

	//***********************************************************
	/**
	* 向一个目标变形，那个目标也是一个形状，或是一个形状的特征。只能操作本级的数据，下级数据需要调用下级形状变形。
	* 连续的变形，就是动画的每一帧。
	*/
	 void morph(Shape* shape);
	 void morph(ShapeFeature* targetFeature);






};

} /* namespace ac */

#endif /* SHAPE_H_ */
