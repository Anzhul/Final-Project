import './style.css'
import {Scene} from './scene.ts'
import * as THREE from 'three';
import { GLTFLoader } from 'three/addons/loaders/GLTFLoader.js';
import { EXRLoader } from 'three/examples/jsm/loaders/EXRLoader.js';
import { OrbitControls } from 'three/addons/controls/OrbitControls.js';


//Get stream data

//If stream data specifies a 

console.log("hello world");
let scene : Scene = new Scene("canvas");
scene.setCameraPosition(0, 0, 10);
window.addEventListener('resize', () => {
    console.log("window resized");
    scene.setRenderSize(window.innerWidth, window.innerHeight);
    scene.render();
})

let exrCubeRenderTarget;
let exrBackground;

const pmremGenerator = new THREE.PMREMGenerator( scene.renderer );
pmremGenerator.compileEquirectangularShader();


new EXRLoader().load('src/assets/moonlit_sky_0025.exr', function (texture: any) {

    texture.mapping = THREE.EquirectangularReflectionMapping;

	exrCubeRenderTarget = pmremGenerator.fromEquirectangular( texture );
	exrBackground = texture;

    scene.addBackground(exrBackground);
    
});


// Instantiate a loader
// Instantiate a loader
const loader = new GLTFLoader();
let camera: any;

// Load a glTF resource
loader.load(
	// resource URL
	'src/assets/camera.glb',
	// called when the resource is loaded
	function ( gltf ) {

		scene.add( gltf.scene );
        camera = gltf.cameras[ 0 ]

	},
	// called while loading is progressing
	function ( xhr ) {

		console.log( ( xhr.loaded / xhr.total * 100 ) + '% loaded' );

	},
	// called when loading has errors
);




scene.render();
//startLoop();


const controls = new OrbitControls( scene.camera, scene.renderer.domElement );

//controls.update() must be called after any manual changes to the camera's transform
scene.camera.position.set( 0, 20, 100 );
controls.update();

function animate() {

	requestAnimationFrame( animate );

	// required if controls.enableDamping or controls.autoRotate are set to true
	controls.update();

	scene.renderer.render( scene.scene, camera );

}
animate();