TAG_NAME="wasmcarts:latest"
PLATFORM="linux/amd64"

docker build --tag $TAG_NAME --platform $PLATFORM .

