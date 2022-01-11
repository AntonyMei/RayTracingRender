# Renderer Usage
___
## Usage on Windows
+ <font size="4"> step 1: compile this project using cmake </font>
+ <font size="4"> step 2: run "Project.exe 8", here 8 means using 8 threads (after running we will get 8 .partial files) </font>
+ <font size="4"> step 3: run "packager.exe 8", here 8 means combining 8 partial files (after running we will get a .ppm image, which can be opened using OpenSeeIt) </font>
+ <font size="4"> step 4: run python convert.py, which converts .ppm into .jpg (note that opencv for python is required to run this) </font>

## Usage on Linux
<font size="4"> run "bash linux_run.sh", which compiles the renderer, uses 80 processes to run it and process result into a .ppm and a .jpg file. (Note that for default Sponza Crytek scene, about 150 GB memory is required. This requirement is proportional to the number of processes.) </font>

\
<font size="4"> **NOTE: For detailed usage and code explanation, see /report/report.pdf.** </font>


# Results
___
## <font size="5"> 1. Hollow Glass Ball </font>

![Hollow Glass Ball](results/hollow_glass_ball.jpg)

<font size="4"> This scene shows the usage of Lambertian (pure diffuse), Metallic (pure reflection) and Dielectric (refraction) material. </font>

## <font size="5"> 2. Hollow Glass Ball Small FOV </font>

![Hollow Glass Ball Small FOV](results/hollow_glass_ball_small_fov.jpg)

<font size="4"> This scene is identical to the last one, but with smaller camera FOV. Note that edge of the dielectric ball has reflection rather than refraction, which is physically accurate. </font>

## <font size="5"> 3. Hollow Glass Ball Off Focus </font>

![Hollow Glass Ball Off Focus](results/hollow_glass_ball_off_focus.jpg)

<font size="4"> This scene shows off-focus blur effect. </font>

## <font size="5"> 4. Many Balls </font>

![Many Balls](results/many_balls.jpg)

<font size="4"> This scene contains many balls. Note that metallic material can use fuzziness to simulate imperfect reflection. </font>

## <font size="5"> 5. Motion Blur </font>

![Motion Blur](results/motion_blur.jpg)

<font size="4"> This scene shows motion blur effect. </font>

## <font size="5"> 6. Motion Blur Checker </font>

![Motion Blur Checker](results/motion_blur_checker.jpg)

<font size="4"> This scene adds a simple procedural texture (checker texture) to the last one. </font>

## <font size="5"> 7. Earth </font>

![Earth](results/earth.jpg)

<font size="4"> This scene is a ball with image texture. </font>

## <font size="5"> 8. Cornell Box Series </font>

![Cornell Box Empty](results/cornell_box_empty.jpg)

<font size="4"> This is an empty Cornell Box constructed with axis-aligned rectangles. </font>

![Cornell Box Two Blocks](results/cornell_box_two_blocks.jpg)

<font size="4"> Added two boxes to the last scene. </font>

![Cornell Box](results/cornell_box.jpg)

<font size="4"> Add rotation to the two boxes. Color bleeding is obvious on two surfaces facing walls. </font>

##<font size="5"> 9. Cornell Box Participating Media </font>

![Cornell Box Participating Media](results/cornell_box_participating_media.jpg)

<font size="4"> This scene replace the two original boxes with participating media boxes. These boxes are assigned with isotropic material to simulate the effect of smoke. This is a technique often used in volumetric rendering. </font>

## <font size="5"> 10. Book2 Final </font>

![Book 2 Final](results/book_2_final.jpg)

<font size="4"> This scene is a modified version of the one used by Peter Shirley in Ray Tracing Mini-Books 2. It combines may techniques. The whole scene is filled with thin smoke, rendered using participating media. The orange ball on the top left shows motion blur. The earth ball shows image textures. The marble ball in the middle is a complex example of procedural texture computed using Perlin noise. The box on the top right contains 10000 balls as components and use BVH to accelerate. </font>

## <font size="5"> 11. Test Obj </font>

![Test Obj](results/test_obj.jpg)

<font size="4"> A simple scene used in obj test. This Cornell Box is made of triangles. It is stored in a .obj file (with material description in .mtl file) and imported using Tinyobjloader. </font>

## <font size="5"> 12. Sponza Sun </font>

![Sponza Sun](results/sponza_sun.jpg)

<font size="4"> This scene is a more complicated one with image textures. Note that in this scene, a biased sampling technique is used such that the sun gets more samples. </font>

## <font size="5"> 13. Sponza Crytek Series </font>

![Sponza Crytek Cloudy](results/sponza_crytek_cloudy.jpg)

<font size="4"> This scene is a remastered version of the last one from Crytek cooperation. It is rendered unbiasedly with Path Tracing. Note that bump map is enabled to add more details to the geometry (like the lion in the back). The following one is the same scene with a more complicated skybox. It can be used to simulate extremely sunny weather. </font>

![Sponza Crytek Sunny](results/sponza_crytek_sunny.jpg)

## <font size="5"> 14. Photon Mapping Series </font>
<font size="4"> A bunny rendered using Photon Mapping. When rendering the first image, photon tracing depth is set to one so that the caustic effect can be seen clearly. The correct effect is shown in the second image. </font>

![Photon Mapping Bunny Caustic](results/photon_mapping_bunny_caustic.jpg)

![Photon Mapping Bunny](results/photon_mapping_bunny.jpg)

