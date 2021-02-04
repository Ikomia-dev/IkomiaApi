#ifndef KERNELS_HPP
#define KERNELS_HPP

#include <string>

static std::string kernelBoxFilter = R"(
                        // Separable kernel code.

                        __kernel void box_filter_horizontal(__global uchar4* inputImage, __global uchar4* outputImage, int filterWidth)
                        {
                            int x = get_global_id(0);
                            int y = get_global_id(1);

                            int width = get_global_size(0);
                            int height = get_global_size(1);

                            int pos = x + y * width;
                            int k = (filterWidth - 1)/2;

                            /* Discard pixels in apron */
                            if(x < k || x >= (width - k))
                            {
                                outputImage[pos] = (uchar4)(0);
                                return;
                            }

                            int4 size = (int4)(filterWidth);

                            int4 sum = 0;
                            /* Read values from (filterWidth x filterWidth) sized kernel */
                            for(int X = -k; X < k; X=X+2)
                            {
                                sum += convert_int4(inputImage[pos + X]);
                                sum += convert_int4(inputImage[pos + X + 1]);
                            }
                            sum += convert_int4(inputImage[pos + k]);
                            outputImage[pos] = convert_uchar4(sum / size);
                        }


                        __kernel void box_filter_vertical(__global uchar4* inputImage, __global uchar4* outputImage, int filterWidth)
                        {
                            int x = get_global_id(0);
                                int y = get_global_id(1);

                            int width = get_global_size(0);
                            int height = get_global_size(1);

                            int pos = x + y * width;
                            int k = (filterWidth - 1)/2;

                            /* Discard pixels in apron */
                            if(y < k || y >= (height - k))
                            {
                                outputImage[pos] = (uchar4)(0);
                                return;
                            }

                            int4 size = (int4)(filterWidth);

                            int4 sum = 0;
                            /* Read values from (filterWidth x filterWidth) sized kernel */
                            for(int Y = -k; Y < k; Y=Y+2)
                            {
                                sum += convert_int4(inputImage[pos + Y * width]);
                                sum += convert_int4(inputImage[pos + (Y + 1) * width]);
                            }
                            sum += convert_int4(inputImage[pos + k * width]);
                            outputImage[pos] = convert_uchar4(sum / size);
                        }
                        )";

#endif // KERNELS_HPP
