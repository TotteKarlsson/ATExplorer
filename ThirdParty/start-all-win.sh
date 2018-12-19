#! /usr/bin/bash
BUILD=${1:-false}
echo "Build flag is "$BUILD

#Render Service, mongo & ndviz stuff
docker-compose -f ./init/docker-compose-windows.yml up -d

#AT MODULES
image_tag="sharmi/at_modules"
container="atmodules"

if [ $BUILD == "true" ]; then
    echo " ======== Building  Docker container: $container =============="

    echo "Building image with tag: $image_tag"
    docker build -t  $image_tag -f ./init/Dockerfile-atmodules ./at_modules
fi    

echo "Starting container: $container"
docker kill $container
docker rm $container

docker run -d --name $container \
-v e:/Documents/data:/mnt/data/  \
-v c:/pDisk/atExplorer/ThirdParty/atPipeline/pipeline:/pipeline  \
-v c:/pDisk/atExplorer/ThirdParty/at_modules/src:/usr/local/at_modules/src  \
-i -t $image_tag /bin/bash 

#RENDER PYTHON APPS ====================================
image_tag="fcollman/render-python-apps"
container="renderapps_multchan"

echo " ======== Building and starting Docker container: $container =============="

if [ $BUILD == "true" ]; then
    echo "Building image with tag: $image_tag"
    docker build -t $image_tag -f ./init/Dockerfile-RenderPythonApps ./Render-Python-Apps
fi    

echo "Starting container: $container"
docker kill $container
docker rm $container 
docker run -d --name $container  \
-v e:/Documents/data:/mnt/data/  \
-v c:/pDisk/atExplorer/ThirdParty/atPipeline/pipeline:/pipeline  \
-v c:/pDisk/atExplorer/ThirdParty/Render-Python-Apps:/usr/local/render-python-apps  \
-i -t $image_tag /bin/bash

echo "Done.."