#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <bits/types/FILE.h>
#include <opencv2/core.hpp>
#include <bits/stdc++.h>
#include "line.hpp"

#define getThresh(x) (((x)*2.+255.*1.)/3.)

cv::Point2f r_tag_img_vertices[4];
cv::Mat r_tag_img;

struct PointStablizer{
    cv::Point2f* q;
    int max_len,len;
    char full=0;
    PointStablizer(int l){
        max_len=l;len=0;
        q=(cv::Point2f*)calloc(max_len,sizeof(cv::Point2f));
    }
    void push(cv::Point2f ipt){
        if (len==max_len-1) full=1;
        q[len]=ipt,len=(len+1)%max_len;
    }
    cv::Point2f get(){
        if ((!full) && len==1) return q[0];
        double max_mse = 0;
        cv::Point2f bad_point,mean(0,0);
        int last=len;
        if (full) last=max_len;
        for (int i=0;i<last;i++) mean+=q[i];
        mean/=last;
        for (int i=0;i<last;i++){
            double mse=cv::norm(q[i]-mean);
            if (max_mse<mse){
                max_mse=mse;
                bad_point=q[i];
            }
        }
        return (mean*last-bad_point)/(last-1);
    }
};

void getRTagImage(){
  cv::Mat channels[3];
  cv::Mat temp = cv::imread("../resource/r_tag.png");
  cv::split(temp,channels);
  cv::Rect cop = cv::Rect(cv::Point(0,0),cv::Point(20,20));
  r_tag_img = channels[2](cop);
  r_tag_img.convertTo(r_tag_img,CV_64F);
  r_tag_img_vertices[0]=cop.tl();
  r_tag_img_vertices[1]=cv::Point(cop.tl().x,cop.br().y);
  r_tag_img_vertices[2]=cop.br();
  r_tag_img_vertices[3]=cv::Point(cop.br().x,cop.tl().y);
  cv::threshold(r_tag_img,r_tag_img,100,255,cv::THRESH_BINARY);
}

void preprocessImage(cv::Mat& frame, cv::Mat& gray, cv::Mat& binary){
    cv::Mat channels[3];
    cv::split(frame,channels);
    gray=channels[2];
    cv::threshold(gray,binary,220,255,cv::THRESH_BINARY);
}

bool findRtag(cv::Mat& binary, cv::Mat& gray, cv::Point2f& rtag){
    std::vector<std::pair<cv::Point2f,double>> rtags;
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    if (contours.empty()) return false;
    cv::Point2f vertices[4];
    cv::Mat perspective;

    for (auto &contour : contours){
        cv::RotatedRect r_rect = cv::minAreaRect(contour);
        if ((r_rect.size.width / r_rect.size.height > 1.3) || (r_rect.size.height / r_rect.size.width > 1.3)) continue;
        cv::Rect rect = cv::boundingRect(contour);

        r_rect.points(vertices);
        cv::Point2f top_left(rect.tl());
        for (int i=0;i<4;i++) vertices[i] -= top_left;
        cv::Mat perspective = gray(rect);
        //cv::warpPerspective(cop, perspective, cv::getPerspectiveTransform(vertices,r_tag_img_vertices),cv::Size(20,20));
        cv::threshold(perspective,perspective,getThresh(cv::mean(perspective)[0]),255,cv::THRESH_BINARY);
        cv::imshow("p",perspective);
        cv::imshow("gray",gray);
        while (cv::waitKey(1)!='q');
    }
    return true;
}

int main() {
    cv::VideoCapture cap("../resource/test_rune.avi");
    getRTagImage();
    std::cout<<r_tag_img.size()<<'\n';

    cv::Mat frame,gray,binary;
    cv::Point2f rtag;
    for (cap>>frame;!frame.empty();cap>>frame){
        preprocessImage(frame,gray,binary);
        findRtag(binary,gray,rtag);

        //cv::imshow("gray",gray);
        //if (cv::waitKey(1)=='q') break;
    }
	return 0;
}