const std = @import("std");
const MyArrayList = @import("./myarraylist.zig").MyArrayList;

const Direction = enum(u2) { North, East, South, West };

const Maze = struct {
    const Self = @This();
    buf: MyArrayList(u8),
    row: usize,
    col: usize,

    pub fn get(self: Self, x: usize, y: usize) u8 {
        return self.buf.items[y * self.col + x];
    }

    pub fn set(self: Self, x: usize, y: usize, val: u8) void {
        self.buf.items[y * self.col + x] = val;
    }

    pub fn addRow(self: *Self, line: []const u8) !void {
        std.debug.assert(line.len == self.col);
        try self.buf.appendSlice(line);
        self.row += 1;
    }

    pub fn init(allocator: std.mem.Allocator, col: usize) Maze {
        return Maze{ .buf = MyArrayList(u8).init(allocator), .row = 0, .col = col };
    }

    pub fn deinit(self: Self) void {
        self.buf.deinit();
    }
};

pub fn mazeSolverRec(allocator: std.mem.Allocator, maze: Maze) ![]Direction {
    var res = MyArrayList(Direction).init(allocator);
    defer res.deinit();

    var i: usize = 0;
    while (maze.buf.items[i] != 'S') : (i += 1) {}

    const start_row = i % maze.col;
    const start_col = i / maze.col;

    const solved = try _mazeSolverRec(maze, &res, start_row, start_col);
    if (!solved) return error.NoPathFound;

    return res.cloneSlice();
}

/// Returns whether it solved it
fn _mazeSolverRec(maze: Maze, res: *MyArrayList(Direction), x: usize, y: usize) !bool {
    // std.debug.print("{d} {d}\n", .{ x, y });
    // for (maze.buf.items, 0..) |tile, idx| {
    //     std.debug.print("{u}", .{tile});
    //     if (idx % maze.col == maze.col - 1) {
    //         std.debug.print("\n", .{});
    //     }
    // }
    // std.debug.print("\n", .{});

    if (maze.get(x, y) == 'E') {
        return true;
    }

    maze.set(x, y, 'x');
    defer maze.set(x, y, '.');

    if (x > 0 and _canWalk(maze, x - 1, y)) {
        try res.append(.West);
        const solved = try _mazeSolverRec(maze, res, x - 1, y);
        if (solved) return true;
        _ = res.pop();
    }
    if (x + 1 < maze.col and _canWalk(maze, x + 1, y)) {
        try res.append(.East);
        const solved = try _mazeSolverRec(maze, res, x + 1, y);
        if (solved) return true;
        _ = res.pop();
    }
    if (y > 0 and _canWalk(maze, x, y - 1)) {
        try res.append(.North);
        const solved = try _mazeSolverRec(maze, res, x, y - 1);
        if (solved) return true;
        _ = res.pop();
    }
    if (y + 1 < maze.row and _canWalk(maze, x, y + 1)) {
        try res.append(.South);
        const solved = try _mazeSolverRec(maze, res, x, y + 1);
        if (solved) return true;
        _ = res.pop();
    }
    return false;
}

fn _canWalk(maze: Maze, x: usize, y: usize) bool {
    const tile = maze.get(x, y);
    return tile == '.' or tile == 'S' or tile == 'E';
}

test "onetile" {
    const allocator = std.testing.allocator;
    var maze = Maze.init(allocator, 2);
    defer maze.deinit();

    try maze.addRow("SE");

    const res = try mazeSolverRec(allocator, maze);
    defer allocator.free(res);
    try std.testing.expectEqualSlices(Direction, &[_]Direction{.East}, res);
}

test "primeagent's maze" {
    const allocator = std.testing.allocator;
    var maze = Maze.init(allocator, 7);
    defer maze.deinit();

    try maze.addRow("#####E#");
    try maze.addRow("#.....#");
    try maze.addRow("#S#####");

    const res = try mazeSolverRec(allocator, maze);
    defer allocator.free(res);

    const expected = [_]Direction{ .North, .East, .East, .East, .East, .North };
    try std.testing.expectEqualSlices(Direction, &expected, res);
}

test "complicated maze" {
    const allocator = std.testing.allocator;
    var maze = Maze.init(allocator, 101);
    defer maze.deinit();

    try maze.addRow("..###################################################################################################");
    try maze.addRow("......#.#.......#.#...#.........#.......#.......#.#.....#...#.....#.......#.#.......#.#.....#.......#");
    try maze.addRow("#.###.#.#.#####.#.#.###.###.###.###.###.#.###.###.###.#####.#.###.#.#.#####.###.#####.#.#.#.#.#.###.#");
    try maze.addRow("#.#.#.....#.#.#...#...#.#.....#.#.....#.....#.#.........#...#...#...#...#.......#...#.#.#.#...#...#.#");
    try maze.addRow("###.#.#.###.#.#.#####.#.#.#.#.#.#######.#########.#.#.#.###.#.#######.###.###.###.###.#####.#####.###");
    try maze.addRow("#.....#.#.#...#.#...#.#.#.#.#.#.....#...#.......#.#.#.#.....#.#.....#...#.#.#...#.......#...#.#...#.#");
    try maze.addRow("###.#####.###.#.#.###.#.#########.#.#.#######.#######.###.#####.#####.#.#.#.#.###.###.###.#.#.#####.#");
    try maze.addRow("#...#...#...#...#...#..S....#.....#...#...#.....#...#.#...#...#.......#.#...#.#...#.....#.#.....#...#");
    try maze.addRow("#.###.###.#####.#.#####.###.#.###.#######.###.#.#.#####.#####.#####.#.#.#####.#.#.###.#####.#####.###");
    try maze.addRow("#...#.......#.#.......#.#.#.#.#.#...#...#.....#...#...#.........#...#.#.....#...#.#...........#...#.#");
    try maze.addRow("#.###.###.###.###.#####.#.#####.#.###.#.#.#####.###.#.#.#.###########.###.#####.#.#####.###.###.###.#");
    try maze.addRow("#...#...#.#.#.#.#.......#...#.......#.#.#...#...#.#.#.#.#.......#.....#.....#...#.#.....#.#.....#...#");
    try maze.addRow("#####.#####.#.#.#.###.#.#.#.#######.###.#######.#.#.#########.###.#####.#.#.#.#######.###.#.#.#.#.###");
    try maze.addRow("#.....#.#.#.#.....#...#...#...#.#...........#.....#...#.......#.....#...#.#.#...#.........#.#.#.....#");
    try maze.addRow("###.###.#.#.#########.#######.#.#.#######.###.#.###.#.#.#.###.#####.###.#####.#.#######.#########.###");
    try maze.addRow("#.#.#.........#...#.#...#.#...#.....#.....#...#.....#...#...#...#...#...#.#.#.#.#...#.#.....#.......#");
    try maze.addRow("#.#.#######.#.#.###.###.#.#.#######.###.###.#.#.###.#.#####.#####.#####.#.#.###.#.###.#############.#");
    try maze.addRow("#...#...#...#.........#.#.#.#...#...#...#...#.#...#.#.....#.#...#.#.#.....#.............#...#.#.#...#");
    try maze.addRow("#.#####.#.#.#.#####.###.#.###.###.#####.###.###################.#.#.#####.#####.#####.#####.#.#.#####");
    try maze.addRow("#.........#.#...#.#...#.....#.........#.....#...#...#.#.....#...#...#.....#.....#...#.#.....#.......#");
    try maze.addRow("###.###.###.#.###.#######.#####.###.#######.###.#.#.#.#.#####.#.#.###########.#####.###.#####.#####.#");
    try maze.addRow("#.....#.#.#.#...............#.....#.#.#...#.#...#.#.........#.#...#.....#.#...#...#.#.....#...#.#...#");
    try maze.addRow("#.#.###.#.#######.###.#############.#.###.#####.#.###.#######.#########.#.###.#.#.#.#.#####.###.###.#");
    try maze.addRow("#.#.#.#...#.#.....#.#.....#.........#.......#.......#.....#...#.....#.....#.....#.#.....#.#.....#...#");
    try maze.addRow("#.###.#.###.#####.#.#####.###.#.###.#.#.#.#############.###.#####.#####.#.#.#.#####.#####.#.###.#.#.#");
    try maze.addRow("#.....#.....#...#.......#.#.#.#.#...#.#.#.#...#.....#.....#.#.#.........#...#.#...#...#.....#...#.#.#");
    try maze.addRow("#.###.#######.#.###.#.#.###.###.#.#.#####.#.#.###.###.###.#.#.#####.#.###########.#.###.#######.###.#");
    try maze.addRow("#.#...#.#.#...#.....#.#.#.#...#.#.#.#.......#.#.........#...........#.....#.#.#.#.......#.......#...#");
    try maze.addRow("###.###.#.#######.#######.###.#.#.###.#######.#########.###.#.#########.###.#.#.###.#####.###########");
    try maze.addRow("#...........#.....#...#.#.#.....#.......#...#.....#...#.#...#.#.#.........#...#.....#...#...#.#...#.#");
    try maze.addRow("#########.###.#######.#.#.###.###.#####.#.#######.#.#.#####.###.###.#########.###.###.###.#.#.#.###.#");
    try maze.addRow("#...#.....#...#.#.......#...#.#...#.#.....#....E#...#.#.....#...#.......................#.#.....#.#.#");
    try maze.addRow("#.#####.###.###.#####.###.###.#.###.#.###.#.#########.#####.#.###.###.#####.#########.###.#.#.###.#.#");
    try maze.addRow("#.#.#...#.#.....#...#.#.....#.#...#.#.#.#...#...#.....#.....#.....#...#.......#.#.#...#...#.#...#.#.#");
    try maze.addRow("#.#.#####.#######.###.#.#.###.#.###.#.#.#######.###.#.###.#.#########.#####.#.#.#.###.#######.#.#.#.#");
    try maze.addRow("#.#...........#.....#...#.....#.#...#.#.....#...#...#.....#.......#.....#...#.....#.#.....#...#.#...#");
    try maze.addRow("#.#.#####.###.#####.#.###########.###.#.#######.#.###.###.#####.###.#.#########.###.#######.#####.#.#");
    try maze.addRow("#.#.#.....#.#.#.#.#...#...#.#.....#.......#.#.......#.#.#...#.#...#.#.....#.....#...#...#.........#.#");
    try maze.addRow("#.#.###.###.#.#.#.#.#.#.###.#####.#.#####.#.#.#####.#.#.###.#.###.#####.#.###.###.#####.#.#####.#.#.#");
    try maze.addRow("#...#.....#.........#.#...............#...#.......#.#.#.......#.....#...#.#.........#.....#.....#.#.#");
    try maze.addRow("###################################################################################################.#");

    const res = try mazeSolverRec(allocator, maze);
    defer allocator.free(res);

    // zig fmt: off
    const expected = [_]Direction{
        .North, .North, .North, .North, .North, .North, 
        .East, .East, .East, .East, .East, .East, .East, .East, 
        .South, .South, .South, .South, 
        .East, .East,
        .South, .South, .South, .South, .South, .South, 
        .East, .East,
        .South, .South,
        .West, .West,
        .South, .South,
        .East, .East,
        .South, .South,
        .West, .West,
        .South, .South,
        .East, .East, 
        .South, .South, .South, .South, .South, .South, 
        .West, .West,
        .South, .South, .South, .South, 
        .East, .East, .East, .East, .East, .East, 
        .South, .South,
        .East, .East,
        .South, .South,
        .East, .East,
        .North, .North,
        .East, .East, .East, .East, 
    };
    // zig fmt: on

    try std.testing.expectEqualSlices(Direction, &expected, res);
}
