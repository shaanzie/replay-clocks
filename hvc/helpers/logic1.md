# Hybrid Vector Clocks Algorithm

- Assumptions
    - Clock drift (e): The maximum drift that can be observed between 2 clocks
    - Discretization (d): The clocks are discrete with modulus d
    - Events happening between discrete stamps of length d are denoted by counters


## HLC layer

```
class HLC {
    int32 seconds;
    int32 subseconds;
}
```

## HVC layer

```
class HVC {
    HLC log_clock;
    string offset_map;
    vector<int> offsets;
    string counter_map;
    vector<int> counters;
}
```

## Wrapper algorithm

```
Algorithm get_process_clock(HVC previous_clock, process_number e, process_number f)
---------------------------------------

HVC current_clock = get_system_clock()
process_number p = get_process_number()

HVC updated_clock = update_clock(previous_clock, current_clock, e, f)

return updated_clock
```

## Update Clock

```
Algorithm update_clock(HVC previous_clock, HVC current_clock, process_number e, process_number f)
----------------------------------------

// Here we have the current_clock with all fields populated

// Update counters
current_clock.counters = previous_clock.counters
if current_clock.log_clock == previous_clock.log_clock:
    current_clock.counters[f] += 1
    current_clock.counter_map[f] = '1'

// Update offsets
vector<int> offset_exist = gather_offsets(previous_clock.offset_map) // O(not null) algorithm
current_clock = set_valid_offsets(current_clock, previous_clock, e, f)

return current_clock
```

```
Algorithm gather_offsets(string offset_map)
-----------------------------------------

vector<int> offsets_exist
int num = stoi(offset_map)
int n = num;
while(n>0){
    int pos = (~(n ^ (~(n - 1))) + 1) >> 1;
    offsets_exist.push_back(pos)
    n = n & (n-1);
}
return offsets_exist

```

```
Algorithm set_valid_offsets(HVC current_clock, HVC previous_clock, process_number e, process_number f)
------------------------------------------

vector<HLC> previous_clocks = get_all_clocks(previous_clock, e) \\ O(not null) algorithm
current_clock.offsets, current_clock.offset_map = construct_offset_map(current_clock.log_clock, previous_clocks) \\ O(not null)

return current_clock
```

```
Algorithm get_all_clocks(HVC previous_clock, process_number e)
------------------------------------------

vector<int> not_null_indexes = get_not_null_idx(previous_clock.offsets) // O(not null)
vector<HLC> previous_clocks_after_offsets = offset_previous_clock(previous_clock, not_null_indexes) // O(not null)

return previous_clocks_after_offsets
```

```
Algorithm offset_previous_clock(HLC previous_clock, vector<int> not_null_idx)
-------------------------------------------

vector<HLC> previous_clocks
for i in not_null_idx:
    previous_clocks[i] = previous_clock.log_clock + previous_clock.offset_map[i]

return previous_clocks
```

Clock comparison can be done by epoch time + counter / counter max to compare two vector clocks where
    vc.e < vc.f <-> vc.e.j <= vc.f.j and there is a k for which vc.e.k < vc.f.j