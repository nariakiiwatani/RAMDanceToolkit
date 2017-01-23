#!/bin/bash

# usage: $ ./submodules.sh


if [ -r .git ]; then

	# submodule init & update
	echo "[start submodule init]"
	git submodule init
	echo "end submodule init."

	echo "[start submodule update]"
	git submodule update
	echo "end submodule update."

else

	# clone from repo
	cd ./addons

	echo "[couldn't found ./.gitmodule. start clone five addons from github repos instead of git submodule update]"

	# ofxBt
	git clone git@github.com:motoishmz/ofxBt.git
	cd ofxBt
	git checkout de1b0cf57bafaa66cc0028a803661bd227597180
	cd ../

	# ofxCv
	git clone git@github.com:kylemcdonald/ofxCv.git
	cd ofxCv
	git checkout 13f08a234fec795bb2114c7a593b74bd9f6588d7
	cd ../

	# ofxDelaunay
	git clone git@github.com:obviousjim/ofxDelaunay.git
	cd ofxDelaunay
	git checkout d87a0b519a9df1aeb7f5d394e60ef1f5928ff786
	cd ../

	# ofxEvent
	git clone git@github.com:YoshitoONISHI/ofxEvent.git
	cd ofxEvent
	git checkout 3cacf827f3af078ddc1ddbcc3fd14a276c4752ff
	cd ../

	# ofxException
	git clone git@github.com:YoshitoONISHI/ofxException.git
	cd  ofxException
	git checkout 2e8fd0b70587655197caa136845e7f07cac357a3
	cd ../

	# ofxInteractivePrimitives
	git clone git@github.com:satoruhiga/ofxInteractivePrimitives.git
	cd ofxInteractivePrimitives
	git checkout d8ba5caf4bea01500a8d756f595fd610d389c6ba
	cd ../

	# ofxMotioner
	git clone git@github.com:YoshitoONISHI/ofxMotioner.git
	cd ofxMotioner
	git checkout 4bf47cf4bce91b5d71a39e88fd5e6a78be3a7f20
	cd ../

	# ofxNodeArray
	git clone git@github.com:nariakiiwatani/ofxNodeArray.git
	cd ofxNodeArray
	git checkout d88df41f27d188dca07624c1cb1326958f58376e
	cd ../

	# ofxQuadWarp
	git clone git@github.com:julapy/ofxQuadWarp.git
	cd ofxQuadWarp
	git checkout aeaa02a3ac84013bac4520bc1c6d1ee291617263
	cd ../

	# ofxUI
	git clone git@github.com:rezaali/ofxUI.git
	cd ofxUI
	git checkout 4145d50dca66791aa8ea6056504647ccebc3e366
	cd ../

	echo "end clone addons"
	cd ../
fi