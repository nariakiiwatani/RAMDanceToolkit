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
	git clone https://github.com/motoishmz/ofxBt.git
	cd ofxBt
	cd ../

	# ofxUI
	git clone https://github.com/rezaali/ofxUI.git
	cd ofxUI
	git checkout 1832f2a
	cd ../

	# ofxInteractivePrimitives
	git clone https://github.com/satoruhiga/ofxInteractivePrimitives.git
	cd ofxInteractivePrimitives
	git checkout 987205e
	cd ../

	# ofxCv
	git clone https://github.com/kylemcdonald/ofxCv.git
	cd ofxCv
	git checkout 88620c5
	cd ../

	# ofxNodeArray
	git clone https://github.com/nariakiiwatani/ofxNodeArray.git
	cd ofxNodeArray
	git checkout d88df41
	cd ../

	# ofxQuadWarp
	git clone http://github.com/julapy/ofxQuadWarp.git
	cd ofxQuadWarp
	git checkout a45ff5e
	cd ../

	# ofxDelaunay
	git clone https://github.com/obviousjim/ofxDelaunay.git
	cd ofxDelaunay
	git checkout 957f902
	cd ../

	# ofxDmx
	git clone https://github.com/kylemcdonald/ofxDmx.git
	cd ofxDmx
	git checkout f84fa27
	cd ../

	# ofxEvent
	git clone https://github.com/YoshitoONISHI/ofxEvent.git
	cd ofxEvent
	git checkout 3cacf82
	cd ../

	# ofxException
	git clone https://github.com/YoshitoONISHI/ofxException.git
	cd  ofxException
	git checkout de4507b
	cd ../

	# ofxMotioner
	git clone https://github.com/YoshitoONISHI/ofxMotioner.git
	cd ofxMotioner
	git checkout develop
	git checkout 392d243
	cd ../

	echo "end clone addons"
	cd ../
fi