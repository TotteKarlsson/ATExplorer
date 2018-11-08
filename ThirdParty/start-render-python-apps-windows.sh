#! /usr/bin/bash

image_tag="fcollman/render-python-apps"
container="renderapps_multchan"
echo " ======== Building and starting Docker container: $container =============="
echo "Building image with tag: $image_tag"
docker build -t $image_tag ./Render-Python-Apps/  
docker rm $container 

echo "Starting container: $container"
docker run -t --name $container  \
-v e:/Documents/data:/mnt/data/  \
-v c:/pDisk/atExplorer/ThirdParty/atPipeline:/pipeline  \
-v c:/pDisk/atExplorer/ThirdParty/Render-Python-Apps:/usr/local/render-python-apps  \
-i -t $image_tag /bin/bash

echo "Docker container $container finished"
