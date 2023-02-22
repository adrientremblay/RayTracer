//
// Created by adrien on 21/02/23.
//

#include <iostream>
#include "RayTracer.h"
#include "simpleppm.h"

RayTracer::RayTracer(nlohmann::json& j) {

}

void RayTracer::run() {
    // drawing an image
    const int image_width = 256;
    const int image_height = 256;
    const int max_value = 255;

    std::vector<double> buffer(3*image_width*image_height);
    for(int j=0;j<image_height;++j){
        for(int i=0;i<image_width;++i){
            if(((i+j)/255)%2==0){
                buffer[3*j*image_width+3*i+0]=1;
                buffer[3*j*image_width+3*i+1]=1;
                buffer[3*j*image_width+3*i+2]=0;
            } else {
                buffer[3*j*image_width+3*i+0]=0;
                buffer[3*j*image_width+3*i+1]=1;
                buffer[3*j*image_width+3*i+2]=1;
            }
        }
    }

    save_ppm("adrien.ppm", buffer, image_width, image_height);
}