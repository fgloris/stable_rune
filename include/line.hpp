#include <opencv2/core.hpp>
#include <bits/stdc++.h>

namespace cv{

    struct PointNode{
        cv::Point pos;
        point_node *a,*b;
    };

    struct Neighborhood{
        int radius;
        std::vector<cv::Point> data;
        Neighborhood(int r){
            radius=r;
        }
    }

    std::vector<cv::Point> neighborhood(cv::Point x){

    }

    //sorting an unordered collection of points into a unilateral order.
    void unilinker(std::vector<cv::Point> data){
        std::vector<cv::Point> res;
        std::vector<PointNode> tmp;
        for (cv::Point p: data){

        }
    }
}
