
class dot{
    x: number;
    y: number;
    color: string;
    constructor(x: number, y: number, color: string){
        this.x = x;
        this.y = y;
        this.color = color;
    }
    draw(ctx: CanvasRenderingContext2D){
        ctx.fillStyle = this.color;
        ctx.fillRect(this.x, this.y, 1, 1);
    }
}