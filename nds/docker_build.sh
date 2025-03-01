TARGET="wasmds-slim"
TAG_NAME="wasmds:slim"
BRANCH="master"
PLATFORM="linux/amd64"

docker build --target $TARGET --tag $TAG_NAME \
        --build-arg BRANCH=$BRANCH --platform $PLATFORM .

