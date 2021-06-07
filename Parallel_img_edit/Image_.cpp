#include "Image_.h"


void black_rainbow_task::run() {
    int ops[3] = { -1,1,0 };
        RGBApixel temp_pix;
        temp_pix.Red = 255;
        temp_pix.Green = 0;
        temp_pix.Blue = 255;

        ops[0] = -1;
        ops[1] = 1;
        ops[2] = 0;
        for (int x = 0; x < picture->TellWidth(); x++) {
            temp_pix.Red += ops[0];
            temp_pix.Green += ops[1];
            temp_pix.Blue += ops[2];
            for (int y = 0; y < 5; y++) {

                if (temp_pix.Red == 0 && ops[0] == -1) {
                    ops[0] = 1;
                    ops[1] = 0;
                    ops[2] = -1;
                }
                else if (temp_pix.Green == 0 && ops[1] == -1) {
                    ops[0] = -1;
                    ops[1] = 1;
                    ops[2] = 0;
                }
                else if (temp_pix.Blue == 0 && ops[2] == -1) {
                    ops[0] = 0;
                    ops[1] = -1;
                    ops[2] = 1;
                }

                RGBApixel itr_pix = picture->GetPixel(x, row * 5 + y);

                if (itr_pix.Red + itr_pix.Green + itr_pix.Blue < 150) {
                    picture->SetPixel(x, row*5 + y, temp_pix);
                }
            }
        }
}


void pixelate_task::run() {
    int rgbValues[3] = { 0,0,0 };
        for (int x = 0; x < picture->TellWidth(); x += pixelsize) {
            for (int a = 0; a < pixelsize; a++) {
                for (int b = 0; b < pixelsize; b++) {
                    if (picture->TellWidth() > x + a && picture->TellHeight() > row * pixelsize + b) {

                        RGBApixel temp_pix = picture->GetPixel(x + a, row * pixelsize + b);

                        rgbValues[0] += temp_pix.Red;
                        rgbValues[1] += temp_pix.Green;
                        rgbValues[2] += temp_pix.Blue;

                    }
                }
            }

            rgbValues[0] = rgbValues[0] / (pixelsize * pixelsize);
            rgbValues[1] = rgbValues[1] / (pixelsize * pixelsize);
            rgbValues[2] = rgbValues[2] / (pixelsize * pixelsize);

            RGBApixel temp_pixx;
            temp_pixx.Red = rgbValues[0];
            temp_pixx.Green = rgbValues[1];
            temp_pixx.Blue = rgbValues[2];

            for (int a = 0; a < pixelsize; a++) {
                for (int b = 0; b < pixelsize; b++) {
                    if (picture->TellWidth() > x + a && picture->TellHeight() > row * pixelsize + b) {
                        picture->SetPixel(x + a, row * pixelsize + b, temp_pixx);
                    }
                }
            }
        }
}

void negative_task::run() {
    for (int x = 0; x < picture->TellWidth(); x++) {
        for (int y = 0; y < 5; y++) {
            RGBApixel temp_pix = picture->GetPixel(x, 5*row+y);
            temp_pix.Red = 255 - temp_pix.Red;
            temp_pix.Green = 255 - temp_pix.Green;
            temp_pix.Blue = 255 - temp_pix.Blue;
            picture->SetPixel(x, row * 5 + y, temp_pix);
        }
    }
}

void rgb_dominant_task::run() {
    for (int x = 0; x < picture->TellWidth(); x++) {
        for (int y = 0; y < 5; y++) {
            
            RGBApixel temppix = picture->GetPixel(x, 5 * row + y);
            temppix.Red /= 2;
            temppix.Green /= 2; //r > g > b
            temppix.Blue /= 2;

            if (temppix.Red >= temppix.Green && temppix.Red >= temppix.Blue) {
                temppix.Red = 255;
            }
            else if (temppix.Blue >= temppix.Green && temppix.Blue >= temppix.Red) {
                temppix.Blue = 255;
            }
            else {
                temppix.Green = 255;
            }

            picture->SetPixel(x, row * 5 + y, temppix);
        }
    }
}

void cartoonify_task::run() {

    for (int x = 0; x < picture->TellWidth(); x++) {
        for (int y = 0; y < 5; y++) {
            RGBApixel temp_pix = picture->GetPixel(x, 5 * row + y);
            temp_pix.Red = int(temp_pix.Red / sharpness) * sharpness;
            temp_pix.Green = int(temp_pix.Green / sharpness) * sharpness;
            temp_pix.Blue = int(temp_pix.Blue / sharpness) * sharpness;
            picture->SetPixel(x, row * 5 + y, temp_pix);
        }
    }
}


void greyify_task::run() {
    int rgb_total;
    float res;
    
    for (int x = 0; x < picture->TellWidth(); x++) {
        for (int y = 0; y < 5; y++) {
            RGBApixel temp_pix = picture->GetPixel(x, row*5 + y);
            rgb_total = temp_pix.Red + temp_pix.Green + temp_pix.Blue;
            res = 255.0 * (float(rgb_total) / 768);
            temp_pix.Red = res;
            temp_pix.Green = res;
            temp_pix.Blue = res;
            picture->SetPixel(x, row * 5 + y, temp_pix);
        }
        
    }
}

void writeBack_task::run() {
    for (int x = 0; x < picture->TellWidth(); x++) {
        for (int y = 0; y < 5; y++) {
            RGBApixel temp_pix;
            temp_pix.Red = gpu_arr[(row * 5 + y) * picture->TellWidth() * 3 + x * 3];
            temp_pix.Green = gpu_arr[(row * 5 + y) * picture->TellWidth() * 3 + x * 3 + 1];
            temp_pix.Blue = gpu_arr[(row * 5 + y) * picture->TellWidth() * 3 + x * 3 + 2];
            picture->SetPixel(x, row * 5 + y, temp_pix);
        }
    }
}

void Image_::setPic(std::string name) {
    imageName = name;
    revert();
}

void Image_::revert() {
    EditedPic.ReadFromFile(std::string("Pictures/" + imageName + ".bmp").c_str());
    lastEdit = ".bmp";
}

//uses hashings
int Image_::CountdifferentColours() {

    std::hash<int> hash_;
    int number;
    for (int y = 0; y < EditedPic.TellHeight(); y++) {
        for (int x = 0; x < EditedPic.TellWidth(); x++) {
            number = EditedPic(x, y)->Red * 1000000 + EditedPic(x, y)->Green * 1000 + EditedPic(x, y)->Blue;
            if (SavedCls[hash_(number)% 16777216] != true) {
                SavedCls[hash_(number)% 16777216] = true;
            }
            
        }
    }
    number = 0;
    for (int i = 0; i < 16777216; i++) {
        if (SavedCls[i] == true) { //increment and set it back for next calc
            number++;
            SavedCls[i] = false;
        }
    }
    return number;
}


void Image_::pixelate(int pixelSize) { //hard gpu func

    int rgbValues[3] = { 0,0,0 };
    for (int y = 0; y < EditedPic.TellHeight(); y += pixelSize) {
        for (int x = 0; x < EditedPic.TellWidth(); x += pixelSize) {

            for (int a = 0; a < pixelSize; a++) {
                for (int b = 0; b < pixelSize; b++) {
                    if (EditedPic.TellWidth() > x + a && EditedPic.TellHeight() > y + b) {
                        rgbValues[0] += EditedPic(x + a, y + b)->Red;
                        rgbValues[1] += EditedPic(x + a, y + b)->Green;
                        rgbValues[2] += EditedPic(x + a, y + b)->Blue;
                    }
                }
            }

            rgbValues[0] = rgbValues[0] / (pixelSize * pixelSize);
            rgbValues[1] = rgbValues[1] / (pixelSize * pixelSize);
            rgbValues[2] = rgbValues[2] / (pixelSize * pixelSize);

            for (int a = 0; a < pixelSize; a++) {
                for (int b = 0; b < pixelSize; b++) {
                    if (EditedPic.TellWidth() > x + a && EditedPic.TellHeight() > y + b) {
                        EditedPic(x + a, y + b)->Red = rgbValues[0];
                        EditedPic(x + a, y + b)->Green = rgbValues[1];
                        EditedPic(x + a, y + b)->Blue = rgbValues[2];
                    }
                }
            }
        }
    }
    lastEdit = "_pixelate.bmp";
    tempnames.push_back(getEditedImgName());
    EditedPic.WriteToFile(std::string("Pictures/temp/" + imageName + "_pixelate.bmp").c_str());

}

void Image_::cartoonify(int sharpness) { //hard gpu func
    
    for (int y = 0; y < EditedPic.TellHeight(); y++) {
        for (int x = 0; x < EditedPic.TellWidth(); x++) {
            EditedPic(x, y)->Red = int(EditedPic(x, y)->Red / sharpness) * sharpness;
            EditedPic(x, y)->Green = int(EditedPic(x, y)->Green / sharpness) * sharpness;
            EditedPic(x, y)->Blue = int(EditedPic(x, y)->Blue / sharpness) * sharpness;
        }
    }
    lastEdit = "_cartoonify.bmp";
    tempnames.push_back(getEditedImgName());
    EditedPic.WriteToFile(std::string("Pictures/temp/" + imageName + "_cartoonify.bmp").c_str());


}

void Image_::greyify() { //gpu func


    int rgb_total;
    float res;
    for (int y = 0; y < EditedPic.TellHeight(); y++) {
        for (int x = 0; x < EditedPic.TellWidth(); x++) {
            rgb_total = EditedPic(x, y)->Red + EditedPic(x, y)->Green + EditedPic(x, y)->Blue;
            res = 255.0 * (float(rgb_total) / 768);
            EditedPic(x, y)->Red = int(res);
            EditedPic(x, y)->Green = int(res);
            EditedPic(x, y)->Blue = int(res);
        }
    }
    lastEdit = "_greyify.bmp";
    tempnames.push_back(getEditedImgName());
    EditedPic.WriteToFile(std::string("Pictures/temp/" + imageName + "_greyify.bmp").c_str());

   
}

void Image_::black_rainbow() { //gpu func 767

    int ops[3] = { -1,1,0 };
    for (int y = 0; y < EditedPic.TellHeight(); y++) {
        RGBApixel temp_pix;
        temp_pix.Red = 255;
        temp_pix.Green = 0;
        temp_pix.Blue = 255;

        ops[0] = -1;
        ops[1] = 1;
        ops[2] = 0;
        for (int x = 0; x < EditedPic.TellWidth(); x++) {

            temp_pix.Red += ops[0];
            temp_pix.Green += ops[1];
            temp_pix.Blue += ops[2];


            if (temp_pix.Red == 0 && ops[0] == -1) {
                ops[0] = 1;
                ops[1] = 0;
                ops[2] = -1;
            }
            else if (temp_pix.Green == 0 && ops[1] == -1) {
                ops[0] = -1;
                ops[1] = 1;
                ops[2] = 0;
            }
            else if (temp_pix.Blue == 0 && ops[2] == -1) {
                ops[0] = 0;
                ops[1] = -1;
                ops[2] = 1;
            }

            if (EditedPic(x, y)->Red + EditedPic(x, y)->Green + EditedPic(x, y)->Blue < 150) {
                //EditedPic.SetPixel(x - 1, y, temp_pix);
                EditedPic.SetPixel(x, y, temp_pix);
                //x += 3;

            }
        }
    }
    lastEdit = "_black_rainbow.bmp";
    tempnames.push_back(getEditedImgName());
    EditedPic.WriteToFile(std::string("Pictures/temp/" + imageName + "_black_rainbow.bmp").c_str());

}

void Image_::RGBdominant() { //hard CPU func

    RGBApixel temppix;

    for (int y = 0; y < EditedPic.TellHeight(); y++) {
        for (int x = 0; x < EditedPic.TellWidth(); x++) {
            temppix = EditedPic.GetPixel(x, y);
            temppix.Red /= 2;
            temppix.Green /= 2; //r > g > b
            temppix.Blue /= 2;
            
            if (EditedPic(x, y)->Red >= EditedPic(x, y)->Green && EditedPic(x, y)->Red >= EditedPic(x, y)->Blue) {
                temppix.Red = 255;
            }
            else if (EditedPic(x, y)->Blue >= EditedPic(x, y)->Green && EditedPic(x, y)->Blue >= EditedPic(x, y)->Red) {
                temppix.Blue = 255;
            }
            else {
                temppix.Green = 255;
            }
            EditedPic.SetPixel(x, y, temppix);
        }
    }
    lastEdit = "_RGBdominant.bmp";
    tempnames.push_back(getEditedImgName());
    EditedPic.WriteToFile(std::string("Pictures/temp/" + imageName + "_RGBdominant.bmp").c_str());

}

void Image_::negative() {
    for (int y = 0; y < EditedPic.TellHeight(); y++) {
        for (int x = 0; x < EditedPic.TellWidth(); x++) {

            EditedPic(x, y)->Red = 255 - EditedPic(x, y)->Red;
            EditedPic(x, y)->Green = 255 - EditedPic(x, y)->Green;
            EditedPic(x, y)->Blue = 255 - EditedPic(x, y)->Blue;
        }
    }
    lastEdit = "_negative.bmp";
    tempnames.push_back(getEditedImgName());
    EditedPic.WriteToFile(std::string("Pictures/temp/" + imageName + "_negative.bmp").c_str());  
}

void Image_::cpu_farm(int calc) {

    switch (calc)
    {
    case 1: //cartoonify
        lastEdit = "_cartoonify.bmp";
        for (int i = 0; i < EditedPic.TellHeight() / 5; i++) {
            taskQ.push(new cartoonify_task(&EditedPic, i,60));
        }
        
        break;

    case 2: //negative
        lastEdit = "_negative.bmp";
        for (int i = 0; i < EditedPic.TellHeight() / 5; i++) {
            taskQ.push(new negative_task(&EditedPic, i));
        }
        break;

    case 3: //black and white
        lastEdit = "_greyify.bmp";
        for (int i = 0; i < EditedPic.TellHeight() / 5; i++) {
            taskQ.push(new greyify_task(&EditedPic, i));
        }
        break;

    case 4: //rgb dominant
        lastEdit = "_RGBdominant.bmp";
        for (int i = 0; i < EditedPic.TellHeight() / 5; i++) {
            taskQ.push(new rgb_dominant_task(&EditedPic, i));
        }
        break;

    case 5: //black_rainbow
        lastEdit = "_black_rainbow.bmp";
        for (int i = 0; i < EditedPic.TellHeight() / 5; i++) {
            taskQ.push(new black_rainbow_task(&EditedPic, i));
        }
        break;

    case 6: //pixelate
        lastEdit = "_pixelate.bmp";
        for (int i = 0; i < EditedPic.TellHeight() / 5; i++) {
            taskQ.push(new pixelate_task(&EditedPic, i,10));
        }
        break;

    case 7: // function to parallely upload gpu array back
        lastEdit = "_negative.bmp";
        for (int i = 0; i < EditedPic.TellHeight() / 5; i++) {
            taskQ.push(new writeBack_task(&EditedPic, i, dynArr));
        }
        break;
    default:
        break;
    }

    

    int nrOfThreads = std::thread::hardware_concurrency();

    for (int i = 0; i < nrOfThreads; i++) {
        threadVect.push_back(std::thread([&] {
            while (true)
            {
                std::unique_lock<std::mutex> lock(mutexLock);
                if (taskQ.empty()) { break; }
                else {
                    taskQ.front()->run();
                    taskQ.pop();
                }

            }
            }));
    }

    for (int i = 0; i < nrOfThreads; i++) {
        threadVect[i].join();
    }
    threadVect.clear();
    
    tempnames.push_back(getEditedImgName());

    EditedPic.WriteToFile(std::string("Pictures/" + getEditedImgName()).c_str());
}


void Image_::setupGPU_data() {

    const int size = EditedPic.TellHeight() * EditedPic.TellWidth() * 3;
    dynArr = new int[size];

    //copies the picture into one long int array
    for (int y = 0; y < EditedPic.TellHeight(); y++) {
        for (int x = 0; x < EditedPic.TellWidth(); x++) {
            dynArr[y * EditedPic.TellWidth() * 3 + x * 3] = EditedPic(x, y)->Red;
            dynArr[y * EditedPic.TellWidth() * 3 + x * 3 + 1] = EditedPic(x, y)->Green;
            dynArr[y * EditedPic.TellWidth() * 3 + x * 3 + 2] = EditedPic(x, y)->Blue;

            
        }
    }

    
}

void Image_::greyify_gpu() {

    lastEdit = "_greyify.bmp";

    array_view<int, 2> img_arr_view_tst(EditedPic.TellHeight(), EditedPic.TellWidth()*3, dynArr);
    
    parallel_for_each(
        img_arr_view_tst.extent,
        [=](index<2> idx) restrict(amp) {
            
            if (idx[1] % 3 == 0) {        //index is on R x-wise        //index is on R x-wise
                img_arr_view_tst[idx] = float(img_arr_view_tst[idx] + img_arr_view_tst[idx[0]][idx[1] + 1] + +img_arr_view_tst[idx[0]][idx[1] + 2])*0.33f;
                img_arr_view_tst[idx[0]][idx[1] + 1] = img_arr_view_tst[idx];
                img_arr_view_tst[idx[0]][idx[1] + 2] = img_arr_view_tst[idx];
            }
        });

}


void Image_::negative_gpu() {

    lastEdit = "_negative.bmp";

    array_view<int, 2> img_arr_view_tst(EditedPic.TellHeight(), EditedPic.TellWidth() * 3, dynArr);

    parallel_for_each(
        img_arr_view_tst.extent,
        [=](index<2> idx) restrict(amp) {

            if (idx[1] % 3 == 0) {        //index is on R x-wise
                img_arr_view_tst[idx] = 255 - img_arr_view_tst[idx];
                img_arr_view_tst[idx[0]][idx[1] + 1] = 255 - img_arr_view_tst[idx[0]][idx[1] + 1];
                img_arr_view_tst[idx[0]][idx[1] + 2] = 255 - img_arr_view_tst[idx[0]][idx[1] + 2];
            }
        });

    
}

void Image_::cartoonify_gpu(int sharpness) {

    lastEdit = "_cartoonify.bmp";

    array_view<int, 2> img_arr_view_tst(EditedPic.TellHeight(), EditedPic.TellWidth() * 3, dynArr);

    parallel_for_each(
        img_arr_view_tst.extent,
        [=](index<2> idx) restrict(amp) {

            if (idx[1] % 3 == 0) {        //index is on R x-wise
                img_arr_view_tst[idx] = int(img_arr_view_tst[idx] / sharpness) * sharpness;
                img_arr_view_tst[idx[0]][idx[1] + 1] = int(img_arr_view_tst[idx[0]][idx[1] + 1] / sharpness) * sharpness;
                img_arr_view_tst[idx[0]][idx[1] + 2] = int(img_arr_view_tst[idx[0]][idx[1] + 2] / sharpness) * sharpness;
            }
        });
}

void Image_::RGBdominant_gpu() {

    lastEdit = "_RGBdominant.bmp";

    array_view<int, 2> img_arr_view_tst(EditedPic.TellHeight(), EditedPic.TellWidth() * 3, dynArr);

    parallel_for_each(
        img_arr_view_tst.extent,
        [=](index<2> idx) restrict(amp) {

            if (idx[1] % 3 == 0) {        //index is on R x-wise
                img_arr_view_tst[idx] /= 2;
                img_arr_view_tst[idx[0]][idx[1] + 1] /= 2;
                img_arr_view_tst[idx[0]][idx[1] + 2] /= 2;
                if (img_arr_view_tst[idx] >= img_arr_view_tst[idx[0]][idx[1] + 1] && img_arr_view_tst[idx] >= img_arr_view_tst[idx[0]][idx[1] + 2]) {
                    img_arr_view_tst[idx] = 255;
                }
                else if (img_arr_view_tst[idx[0]][idx[1] + 2] >= img_arr_view_tst[idx[0]][idx[1] + 1] && img_arr_view_tst[idx[0]][idx[1] + 2] >= img_arr_view_tst[idx]) {
                    img_arr_view_tst[idx[0]][idx[1] + 2] = 255;
                }
                else {
                    img_arr_view_tst[idx[0]][idx[1] + 1] = 255;
                }
            }

        });


}

void Image_::pixelate_gpu(int pixelsize) {

    lastEdit = "_pixelate.bmp";

    array_view<int, 2> img_arr_view_tst(EditedPic.TellHeight(), EditedPic.TellWidth() * 3, dynArr);

    int size_x = EditedPic.TellWidth() * 3;
    int size_y = EditedPic.TellHeight();
    parallel_for_each(
        img_arr_view_tst.extent,
        [=](index<2> idx) restrict(amp) {

            if (idx[0] % pixelsize == 0 && idx[1] % (pixelsize * 3) == 0) {

                int rgbValues[3] = { 0,0,0 };
                    for (int a = 0; a < pixelsize; a++) {
                        for (int b = 0; b < pixelsize; b++) {
                            if (size_x > idx[1] + a && size_y > idx[0] + b) {

                                rgbValues[0] += img_arr_view_tst[idx[0] + b][idx[1] + a * 3];//R
                                rgbValues[1] += img_arr_view_tst[idx[0] + b][idx[1] + a * 3 + 1];//G
                                rgbValues[2] += img_arr_view_tst[idx[0] + b][idx[1] + a * 3 + 2];//B

                            }
                        }
                    }

                    rgbValues[0] = rgbValues[0] / (pixelsize * pixelsize);
                    rgbValues[1] = rgbValues[1] / (pixelsize * pixelsize);
                    rgbValues[2] = rgbValues[2] / (pixelsize * pixelsize);



                    for (int a = 0; a < pixelsize; a++) {
                        for (int b = 0; b < pixelsize; b++) {
                            if (size_x > idx[1] + a && size_y > idx[0] + b) {

                                img_arr_view_tst[idx[0] + b][idx[1] + a * 3] = rgbValues[0];
                                img_arr_view_tst[idx[0] + b][idx[1] + a * 3 + 1] = rgbValues[1];
                                img_arr_view_tst[idx[0] + b][idx[1] + a * 3 + 2] = rgbValues[2];

                            }
                        }
                    }
                }
        });

}
 
//ineff func cuz not pixel based but line based and cant rly pass it as lines
void Image_::black_rainbow_gpu() { // ONLUY LINEBASED GPU FUNCTION

    lastEdit = "_black_rainbow.bmp";

    array_view<int, 2> img_arr_view_tst(EditedPic.TellHeight(), EditedPic.TellWidth() * 3, dynArr);

    int arr_width = EditedPic.TellWidth() * 3;

    parallel_for_each(
        img_arr_view_tst.extent,
        [=](index<2> idx) restrict(amp) {
            if (idx[1] == 0) {
                int ops[3] = { -1,1,0 };
                int rgb_pix[3] = { 255,0,255 };
                rgb_pix[0] = 255;
                rgb_pix[1] = 0;
                rgb_pix[2] = 255;

                ops[0] = -1;
                ops[1] = 1;
                ops[2] = 0;
                for (int x = 0; x < arr_width; x += 3) {


                    rgb_pix[0] += ops[0];
                    rgb_pix[1] += ops[1];
                    rgb_pix[2] += ops[2];


                    if (rgb_pix[0] == 0 && ops[0] == -1) {
                        ops[0] = 1;
                        ops[1] = 0;
                        ops[2] = -1;
                    }
                    else if (rgb_pix[1] == 0 && ops[1] == -1) {
                        ops[0] = -1;
                        ops[1] = 1;
                        ops[2] = 0;
                    }
                    else if (rgb_pix[2] == 0 && ops[2] == -1) {
                        ops[0] = 0;
                        ops[1] = -1;
                        ops[2] = 1;
                    }

                    if (img_arr_view_tst[idx[0]][x] + img_arr_view_tst[idx[0]][x + 1] + img_arr_view_tst[idx[0]][x + 2] < 150) {
                        img_arr_view_tst[idx[0]][x] = rgb_pix[0];
                        img_arr_view_tst[idx[0]][x + 1] = rgb_pix[1];
                        img_arr_view_tst[idx[0]][x + 2] = rgb_pix[2];
                    }
                }
            }
        });
}