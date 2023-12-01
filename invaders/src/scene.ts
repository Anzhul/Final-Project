import * as THREE from 'three';

export class Scene{
    scene: THREE.Scene;
    camera: THREE.PerspectiveCamera;
    renderer: THREE.WebGLRenderer;
    constructor(renderElement: string){
        this.scene = new THREE.Scene();
        this.camera = new THREE.PerspectiveCamera( 75, window.innerWidth / window.innerHeight, 0.1, 1000 );
        let canvas =   document.getElementById(renderElement)
        this.renderer = new THREE.WebGLRenderer({ canvas: canvas as HTMLCanvasElement });
        this.renderer.setSize( window.innerWidth, window.innerHeight );
    }
    setRenderSize(width: number, height: number){
        this.renderer.setSize(width, height);
    }
    setCameraPosition(x: number, y: number, z: number){
        this.camera.position.set(x, y, z);
    }
    addMesh(mesh: THREE.Mesh){
        this.scene.add(mesh);
    }
    render(){
        this.renderer.render(this.scene, this.camera);
    }

}