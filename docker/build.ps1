#
# Powershell script to build docker image:
#
$image = (Get-Content -Path ".\docker\_image-name.txt" -First 1)
#
# remove previous image:
#
docker image inspect $image *>$null
if ($LASTEXITCODE -eq 0) {
  docker rmi $image
}
#
# build new image:
#
docker build -t $image .\docker
#
# done!
#
echo ""
echo "Build successful, to run type './docker/run'"
echo "The prompt should change, and you will be"
echo "working inside a Linux-based environment."
echo "When you are ready to exit and return to your"
echo "local environment, type 'exit'"
echo "" 