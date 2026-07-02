
```cpp
void handleMelee(Unit* units[], int numUnits){
    for(int a=0;a<numUnits-1;++a){
        for(int b=a+1;b<numUnits;++b){
            if(units[a]->position()==units[b].position()){
                handleAttack(units[a], units[b]);
            }
        }
    }
}
```

```cpp
class Unit{
    friend class Grid;

public:
    Unit(Grid* grid, double x, double y)
    : grid_(grid),
      x_(x),
      y_(y),
      prev_(nullptr),
      next_(nullptr)
    {
        grid_->add(this);
    }

    void move(double x, double y);

private:
    double x_, y_;
    Grid* grid_;

    Unit* prev_;
    Unit* next_;
};
```
```cpp
class Grid{
public:
    Grid(){
        for(int x=0;x<NUM_CELLS;++x){
            for(int y=0;x<NUM_CELLS;++y){
                cells_[x][y]=nullptr;
            }
        }
    }

    void add(Unit* unit);

    static const int NUM_CELLS=10;
    static const int CELL_SIZE=20;
private:
    Unit* cells_[NUM_CELLS][NUM_CELLS];
};
```
```cpp
void Grid::add(Unit* unit){
    int cellX=(int)(unit->x_/Grid::CELL_SIZE);
    int cellY=(int)(unit->y_/Grid::CELL_SIZE);

    unit->prev_=nullptr;
    unit->next_=cells_[cellX][cellY];
    cells_[cellX][cellY]=unit;
    
    if(unit->next_){
        unit->next_->prev_=unit;
    }
}
```

```cpp
void Grid::handleMelee(){
    for(int x=0;x<NUM_CELLS;++x){
        for(int y=0;y<NUM_CELLS;++y){
            handleCell(cells[x][y]);
        }
    }
}
```
```cpp
void Grid::handleCell(Unit* unit){
    while(unit){
        Unit* other=unit->next_;
        while(other){
            if(unit->x_==other->x_&&
               unit->y_==other->y_){
                handleAttack(unit, other);
            }
            other=other->next_;
        }
        unit=unit->next_;
    }
}
```

```cpp
void Unit::move(double x, double y){
    grid_->move(this, x, y);
}
```
```cpp
void Grid::move(Unit* unit, double x, double y){
    int oldCellX=(int)(unit->x_/Grid::CELL_SIZE);
    int oldCellY=(int)(unit->y_/Grid::CELL_SIZE);

    int cellX=(int)(x/Grid::CELL_SIZE);
    int cellY=(int)(y/Grid::CELL_SIZE);

    unit->x_=x;
    unit->y_=y;

    if(oldCellX==cellX&&oldCellY==cellY) return;

    if(unit->prve_) unit->prev_->next_=unit->next_;
    if(unit->next_) unit->next_->prev_=unit->prev_;

    if(cells[oldCellX][oldCellY]==unit){
        cels[oldCellX][oldCellY]=unit->next_;
    }

    add(unit);
}
```

```cpp
if(distance(unit, other)<ATTACK_DISTANCE){
    handleAttack(unit, other);
}
```

```cpp
void Grid::handleUnit(Unit* unit, Unit* other){
    while(other){
        if(distance(unit, other)<ATTACK_DISTANCE){
            handleAttack(unit, other);
        }

        other=other->next_;
    }
}
```
```cpp
void Grid::handleCell(int x, int y){
    Unit* unit=cells[x][y];
    while(unit){
        hanleUnit(unit, unit->next_);

        // 检查一半的相邻格子
        if(x>0&&y>0) handleUnit(unit, sells_[x-1][y-1]);
        if(x>0) handleUnit(unit, sells_[x-1][y]);
        if(y>0) handleUnit(unit, sells_[x][y-1]);
        if(x>0&&y<NUM_CELLS-1){
            handleUnit(unit, sells_[x-1][y+1]);
        }
    }

    unit=unit->next_;
}
```