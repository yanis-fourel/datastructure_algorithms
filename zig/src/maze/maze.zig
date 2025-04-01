const std = @import("std");
const Allocator = std.mem.Allocator;
const AnyReader = std.io.AnyReader;

const IntReader = struct {
    const Self = @This();
    reader: std.io.AnyReader,
    buf: [16]u8 = undefined,

    pub fn init(reader: std.io.AnyReader) Self {
        return Self{ .reader = reader };
    }

    pub fn next(self: *Self) !u32 {
        const word = try self.nextWord();
        return try std.fmt.parseInt(u32, word, 0);
    }

    pub fn nextWord(self: *Self) ![]u8 {
        var i: u8 = 0;
        var didReadWord = false;
        while (true) {
            const byte = try self.reader.readByte();
            if (std.ascii.isWhitespace(byte)) {
                if (didReadWord) {
                    break;
                } else {
                    continue;
                }
            }
            didReadWord = true;
            self.buf[i] = byte;
            i += 1;
        }
        return self.buf[0..i];
    }
};

const Maze = struct {
    const Self = @This();
    width: u32,
    height: u32,
    start_x: u32,
    start_y: u32,
    end_x: u32,
    end_y: u32,
    data: []u8,
    allocator: Allocator,

    pub fn deserialize(allocator: Allocator, reader: AnyReader) !Maze {
        var intReader = IntReader.init(reader);

        const width = try intReader.next();
        const height = try intReader.next();
        const start_x = try intReader.next();
        const start_y = try intReader.next();
        const end_x = try intReader.next();
        const end_y = try intReader.next();

        var data = try allocator.alloc(u8, width * height);
        errdefer allocator.free(data);

        var i: u32 = 0;
        while (i < data.len) {
            const byte = try reader.readByte();
            if (std.ascii.isWhitespace(byte)) continue;
            data[i] = byte;
            i += 1;
        }

        return Maze{
            .width = width,
            .height = height,
            .start_x = start_x,
            .start_y = start_y,
            .end_x = end_x,
            .end_y = end_y,
            .data = data,
            .allocator = allocator,
        };
    }

    // pub fn serialize(writer: Writer) !void {}

    pub fn deinit(self: Self) void {
        self.allocator.free(self.data);
    }
};

test "deserialize" {
    const buf =
        \\4 3
        \\0 1
        \\1 0
        \\#.##
        \\...#
        \\####
    ;

    var fis = std.io.fixedBufferStream(buf);
    const reader = fis.reader();

    const maze = try Maze.deserialize(std.testing.allocator, reader.any());
    defer maze.deinit();

    try std.testing.expectEqual(4, maze.width);
    try std.testing.expectEqual(3, maze.height);
}
