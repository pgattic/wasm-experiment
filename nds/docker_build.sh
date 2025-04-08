TARGET="wasmcarts-dev"
TAG_NAME="wasmcarts:latest"
BRANCH="master"
PLATFORM="linux/amd64"

docker build --target $TARGET --tag $TAG_NAME \
        --build-arg BRANCH=$BRANCH --platform $PLATFORM .

