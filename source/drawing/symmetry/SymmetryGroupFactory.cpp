#include "SymmetryGroupFactory.h"

#include <iomanip>

#include "TrivialGroup.h"
#include "WallpaperGroup.h"
#include "core/Logger.h"


SymmetryGroupFactory* SymmetryGroupFactory::instance = nullptr;

SymmetryGroupFactory& SymmetryGroupFactory::get_instance() {
	if (instance == nullptr)
		instance = new SymmetryGroupFactory();
	return *instance;
}

SymmetryGroupFactory::SymmetryGroupFactory() {
	// vertices for parallelogram cells
	bottom_left  = { "(-v1_x - v2_x) / 2", "(-v1_y - v2_y) / 2" };
	center_left  = { " -v1_x         / 2", " -v1_y         / 2" };
	top_left     = { "(-v1_x + v2_x) / 2", "(-v1_y + v2_y) / 2" };
	bottom_mid   = { "        -v2_x  / 2", "        -v2_y  / 2" };
	origin       = { 0, 0 };
	top_mid      = { "         v2_x  / 2", "         v2_y  / 2" };
	bottom_right = { "( v1_x - v2_x) / 2", "( v1_y - v2_y) / 2" };
	center_right = { "  v1_x         / 2", "  v1_y         / 2" };
	top_right    = { "( v1_x + v2_x) / 2", "( v1_y + v2_y) / 2" };

	// midlines for parallelogram cells
	horizontal_midline = { center_left, center_right };
	vertical_midline = { bottom_mid, top_mid };

	// vertices for hexagon cells
	hex_p0 = { "( 2 * v1_x -     v2_x) / 3", "( 2 * v1_y -     v2_y) / 3" };
	hex_p1 = { "(     v1_x +     v2_x) / 3", "(     v1_y +     v2_y) / 3" };
	hex_p2 = { "(    -v1_x + 2 * v2_x) / 3", "(    -v1_y + 2 * v2_y) / 3" };
	hex_p3 = { "(-2 * v1_x +     v2_x) / 3", "(-2 * v1_y +     v2_y) / 3" };
	hex_p4 = { "(    -v1_x -     v2_x) / 3", "(    -v1_y -     v2_y) / 3" };
	hex_p5 = { "(     v1_x - 2 * v2_x) / 3", "(     v1_y - 2 * v2_y) / 3" };

	hex_p6 = { "(     v1_x +     v2_x) / 6", "(     v1_y +     v2_y) / 6" };
	hex_p7 = { "(-2 * v1_x +     v2_x) / 6", "(-2 * v1_y +     v2_y) / 6" };
	hex_p8 = { "(     v1_x - 2 * v2_x) / 6", "(     v1_y - 2 * v2_y) / 6" };
}

std::shared_ptr<TrivialGroup> SymmetryGroupFactory::trivial() {
	return std::make_shared<TrivialGroup>();
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p1(std::shared_ptr<SymmetryGroup> old) {
	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());	// identity transformation

	std::vector<SymbolicPoint> drawing_area = {
		bottom_left, top_left, top_right, bottom_right
	};

	DomainBoundaries edges;
	edges.add_type_A({ bottom_left, top_left });
	edges.add_type_A({ bottom_right, top_right });
	edges.add_type_B({ bottom_left, bottom_right });
	edges.add_type_B({ top_left, top_right });

	return std::make_shared<WallpaperGroup>(
		"p1",
		WallpaperGroup::PARALLELOGRAM,
		cell,
		drawing_area,
		std::vector<SymbolicPoint>{},
		edges,
		old
	);
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p2(std::shared_ptr<SymmetryGroup> old) {
	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	cell.push_back(SymbolicMatrix::rotate(180, origin));

	std::vector<SymbolicPoint> drawing_area = {
		bottom_left, top_left, top_right, bottom_right
	};

	std::vector<SymbolicPoint> centers {
		top_left, top_mid, top_right,
			center_left, origin, center_right,
			bottom_left, bottom_mid, bottom_right,
	};

	DomainBoundaries edges;
	edges.add_type_A({ bottom_left, center_left });
	edges.add_type_A({ top_left, center_left });
	edges.add_type_A({ bottom_right, center_right });
	edges.add_type_A({ top_right, center_right });
	edges.add_type_B({ bottom_left, bottom_mid });
	edges.add_type_B({ bottom_right, bottom_mid });
	edges.add_type_B({ top_left, top_mid });
	edges.add_type_B({ top_right, top_mid });
	edges.add_type_C({ center_left, origin });
	edges.add_type_C({ center_right, origin });

	return std::make_shared<WallpaperGroup>(
		"p2",
		WallpaperGroup::PARALLELOGRAM,
		cell,
		drawing_area,
		centers,
		edges,
		old
	);
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::pm(std::shared_ptr<SymmetryGroup> old) {
	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	cell.push_back(SymbolicMatrix::reflect(horizontal_midline));

	std::vector<SymbolicPoint> drawing_area = {
		bottom_left, top_left, top_right, bottom_right
	};

	DomainBoundaries edges;
	edges.add_mirror_line({ bottom_left, bottom_right });
	edges.add_mirror_line({ center_left, center_right });
	edges.add_mirror_line({ top_left, top_right });
	edges.add_type_A({ bottom_left, center_left });
	edges.add_type_A({ bottom_right, center_right });
	edges.add_type_A_mirror({ top_left, center_left });
	edges.add_type_A_mirror({ top_right, center_right });

	return std::make_shared<WallpaperGroup>(
		"pm",
		WallpaperGroup::RECTANGLE,
		cell,
		drawing_area,
		std::vector<SymbolicPoint>{},
		edges,
		old
	);
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::pg(std::shared_ptr<SymmetryGroup> old) {
	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	cell.push_back(SymbolicMatrix::reflect(vertical_midline) * SymbolicMatrix::translate(top_mid));

	std::vector<SymbolicPoint> drawing_area = {
		bottom_left, center_left, center_right, bottom_right
	};

	DomainBoundaries edges;
	edges.add_type_A({ bottom_left, bottom_right });
	edges.add_type_A({ top_left, top_right });
	edges.add_type_A_mirror({ center_right, center_left });
	edges.add_type_B({ bottom_left, center_left });
	edges.add_type_B({ bottom_right, center_right });
	edges.add_type_B_mirror({ center_left, top_left });
	edges.add_type_B_mirror({ center_right, top_right });

	return std::make_shared<WallpaperGroup>(
		"pg",
		WallpaperGroup::RECTANGLE,
		cell,
		drawing_area,
		std::vector<SymbolicPoint>{},
		edges,
		old
	);
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::cm(std::shared_ptr<SymmetryGroup> old) {
	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	cell.push_back(SymbolicMatrix::reflect({ bottom_left, top_right }));

	std::vector<SymbolicPoint> drawing_area = {
		bottom_left, top_right, bottom_right
	};

	DomainBoundaries edges;
	edges.add_mirror_line({ bottom_left, top_right });
	edges.add_type_A({ bottom_left, bottom_right });
	edges.add_type_A({ top_left, top_right });
	edges.add_type_A_mirror({ bottom_left, top_left });
	edges.add_type_A_mirror({ bottom_right, top_right });

	return std::make_shared<WallpaperGroup>(
		"cm",
		WallpaperGroup::RHOMBUS,
		cell,
		drawing_area,
		std::vector<SymbolicPoint>{},
		edges,
		old
	);
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::pmm(std::shared_ptr<SymmetryGroup> old) {
	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	cell.push_back(SymbolicMatrix::reflect(vertical_midline));
	cell.push_back(SymbolicMatrix::reflect(horizontal_midline));
	cell.push_back(SymbolicMatrix::reflect(vertical_midline) * SymbolicMatrix::reflect(horizontal_midline));

	std::vector<SymbolicPoint> drawing_area = {
		bottom_left, top_left, top_right, bottom_right
	};

	std::vector<SymbolicPoint> centers {
		top_left, top_mid, top_right,
			center_left, origin, center_right,
			bottom_left, bottom_mid, bottom_right,
	};

	DomainBoundaries edges;
	edges.add_mirror_line({ bottom_left, bottom_right });
	edges.add_mirror_line({ center_left, center_right });
	edges.add_mirror_line({ top_left, top_right });
	edges.add_mirror_line({ bottom_left, top_left });
	edges.add_mirror_line({ bottom_mid, top_mid });
	edges.add_mirror_line({ bottom_right, top_right });

	return std::make_shared<WallpaperGroup>(
		"pmm",
		WallpaperGroup::RECTANGLE,
		cell,
		drawing_area,
		centers,
		edges,
		old
	);
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::pmg(std::shared_ptr<SymmetryGroup> old) {
	SymbolicMatrix vertical_glide = SymbolicMatrix::reflect(vertical_midline) * SymbolicMatrix::translate(top_mid);
	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	cell.push_back(vertical_glide);
	cell.push_back(SymbolicMatrix::reflect(horizontal_midline));
	cell.push_back(vertical_glide * SymbolicMatrix::reflect(horizontal_midline));

	std::vector<SymbolicPoint> drawing_area = {
		bottom_left, center_left, center_right, bottom_right
	};

	SymbolicPoint one_quarter_left    = { "-v1_x / 2 - v2_x / 4", "-v1_y / 2 - v2_y / 4" };	// one quarter of the way from bottom left to top left
	SymbolicPoint three_quarter_left  = { "-v1_x / 2 + v2_x / 4", "-v1_y / 2 + v2_y / 4" };
	SymbolicPoint one_quarter_mid     = { "           -v2_x / 4", "           -v2_y / 4" };
	SymbolicPoint three_quarter_mid   = { "            v2_x / 4", "            v2_y / 4" };
	SymbolicPoint one_quarter_right   = { " v1_x / 2 - v2_x / 4", " v1_y / 2 - v2_y / 4" };
	SymbolicPoint three_quarter_right = { " v1_x / 2 + v2_x / 4", " v1_y / 2 + v2_y / 4" };

	std::vector<SymbolicPoint> centers {
		three_quarter_left, three_quarter_mid, three_quarter_right,
			one_quarter_left, one_quarter_mid, one_quarter_right,
	};

	DomainBoundaries edges;
	edges.add_mirror_line({ bottom_left, bottom_right });
	edges.add_mirror_line({ center_left, center_right });
	edges.add_mirror_line({ top_left, top_right });
	edges.add_type_A({ bottom_left, one_quarter_left });
	edges.add_type_A({ center_left, one_quarter_left });
	edges.add_type_A({ bottom_right, one_quarter_right });
	edges.add_type_A({ center_right, one_quarter_right });
	edges.add_type_A_mirror({ top_left, three_quarter_left });
	edges.add_type_A_mirror({ center_left, three_quarter_left });
	edges.add_type_A_mirror({ top_right, three_quarter_right });
	edges.add_type_A_mirror({ center_right, three_quarter_right });
	edges.add_type_B({ bottom_mid, one_quarter_mid });
	edges.add_type_B({ origin, one_quarter_mid });
	edges.add_type_B_mirror({ top_mid, three_quarter_mid });
	edges.add_type_B_mirror({ origin, three_quarter_mid });

	return std::make_shared<WallpaperGroup>(
		"pmg",
		WallpaperGroup::RECTANGLE,
		cell,
		drawing_area,
		centers,
		edges,
		old
	);
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::pgg(std::shared_ptr<SymmetryGroup> old) {
	SymbolicPoint one_quarter_left   = { "-v1_x / 2 - v2_x / 4", "-v1_y / 2 - v2_y / 4" };	// one quarter of the way from bottom left to top left
	SymbolicPoint one_quarter_mid    = { "           -v2_x / 4", "           -v2_y / 4" };

	auto horizontal_mirror = SymbolicMatrix::reflect({ one_quarter_left, one_quarter_mid });
	auto horizontal_translate = SymbolicMatrix::translate(center_right);
	SymbolicMatrix horizontal_glide = horizontal_mirror * horizontal_translate;
	SymbolicMatrix rotate_180 = SymbolicMatrix::rotate(180, origin);

	//std::unordered_map<std::string, float> variables = { {"v1_x", 1.0f}, {"v1_y", 0.0f}, {"v2_x", 0.0f}, {"v2_y", 1.0f} };
	//*Logger::get_instance() << "horizontal mirror:" << std::endl << horizontal_mirror.evaluate(variables) << std::endl;
	//*Logger::get_instance() << "horizontal translate:" << std::endl << horizontal_translate.evaluate(variables) << std::endl;
	//*Logger::get_instance() << "horizontal glide:" << std::endl << horizontal_glide.evaluate(variables) << std::endl;

	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	cell.push_back(horizontal_glide);
	cell.push_back(rotate_180);
	cell.push_back(horizontal_glide * rotate_180);

	std::vector<SymbolicPoint> drawing_area = {
		bottom_left, center_left, origin, bottom_mid
	};

	std::vector<SymbolicPoint> centers {
		top_left, top_mid, top_right,
		center_left, origin, center_right,
		bottom_left, bottom_mid, bottom_right,
	};

	DomainBoundaries edges;
	edges.add_type_A({ bottom_left, center_left });
	edges.add_type_A({ top_left, center_left });
	edges.add_type_A({ bottom_right, center_right });
	edges.add_type_A({ top_right, center_right });
	edges.add_type_A_mirror({ origin, bottom_mid });
	edges.add_type_A_mirror({ origin, top_mid });
	edges.add_type_B({ bottom_left, bottom_mid });
	edges.add_type_B({ bottom_right, bottom_mid });
	edges.add_type_B({ top_left, top_mid });
	edges.add_type_B({ top_right, top_mid });
	edges.add_type_B_mirror({ origin, center_left });
	edges.add_type_B_mirror({ origin, center_right });

	return std::make_shared<WallpaperGroup>(
		"pgg",
		WallpaperGroup::RECTANGLE,
		cell,
		drawing_area,
		centers,
		edges,
		old
	);

}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::cmm(std::shared_ptr<SymmetryGroup> old) {
	SymbolicMatrix mirror_one = SymbolicMatrix::reflect({ bottom_left, top_right });
	SymbolicMatrix mirror_two = SymbolicMatrix::reflect({ top_left, bottom_right });

	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	cell.push_back(mirror_one);
	cell.push_back(mirror_two);
	cell.push_back(mirror_one * mirror_two);

	std::vector<SymbolicPoint> drawing_area = {
		bottom_left, origin, bottom_right
	};

	std::vector<SymbolicPoint> centers {
		top_left, top_mid, top_right,
		center_left, origin, center_right,
		bottom_left, bottom_mid, bottom_right,
	};

	DomainBoundaries edges;
	edges.add_mirror_line({ bottom_left, top_right });
	edges.add_mirror_line({ top_left, bottom_right });
	edges.add_type_A({ bottom_left, center_left });
	edges.add_type_A({ top_left, center_left });
	edges.add_type_A({ bottom_right, center_right });
	edges.add_type_A({ top_right, center_right });
	edges.add_type_A_mirror({ bottom_left, bottom_mid });
	edges.add_type_A_mirror({ bottom_right, bottom_mid });
	edges.add_type_A_mirror({ top_left, top_mid });
	edges.add_type_A_mirror({ top_right, top_mid });

	return std::make_shared<WallpaperGroup>(
		"cmm",
		WallpaperGroup::RHOMBUS,
		cell,
		drawing_area,
		centers,
		edges,
		old
	);
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p4(std::shared_ptr<SymmetryGroup> old) {
	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	cell.push_back(SymbolicMatrix::rotate(90, origin));
	cell.push_back(SymbolicMatrix::rotate(180, origin));
	cell.push_back(SymbolicMatrix::rotate(270, origin));

	std::vector<SymbolicPoint> drawing_area = {
		bottom_left, top_left, top_right, bottom_right
	};

	std::vector<SymbolicPoint> centers = {
		bottom_left, top_left, top_right, bottom_right, origin
	};

	DomainBoundaries edges;
	edges.add_type_A({ bottom_left, center_left });
	edges.add_type_A({ top_left, center_left });
	edges.add_type_A({ top_left, top_mid });
	edges.add_type_A({ top_right, top_mid });
	edges.add_type_A({ top_right, center_right });
	edges.add_type_A({ bottom_right, center_right });
	edges.add_type_A({ bottom_right, bottom_mid });
	edges.add_type_A({ bottom_left, bottom_mid });
	edges.add_type_B({ center_left, origin });
	edges.add_type_B({ top_mid, origin });
	edges.add_type_B({ center_right, origin });
	edges.add_type_B({ bottom_mid, origin });

	return std::make_shared<WallpaperGroup>(
		"p4",
		WallpaperGroup::SQUARE,
		cell,
		drawing_area,
		centers,
		edges,
		old
	);
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p4m(std::shared_ptr<SymmetryGroup> old) {
	SymbolicMatrix mirror = SymbolicMatrix::reflect({ bottom_left, origin });

	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	cell.push_back(SymbolicMatrix::rotate(90, origin));
	cell.push_back(SymbolicMatrix::rotate(180, origin));
	cell.push_back(SymbolicMatrix::rotate(270, origin));
	cell.push_back(mirror);
	cell.push_back(mirror * SymbolicMatrix::rotate(90, origin));
	cell.push_back(mirror * SymbolicMatrix::rotate(180, origin));
	cell.push_back(mirror * SymbolicMatrix::rotate(270, origin));

	std::vector<SymbolicPoint> drawing_area = {
		bottom_left, top_left, top_right, bottom_right
	};

	std::vector<SymbolicPoint> centers {
		top_left, top_mid, top_right,
		center_left, origin, center_right,
		bottom_left, bottom_mid, bottom_right,
	};

	DomainBoundaries edges;
	edges.add_mirror_line({ bottom_left, top_left });
	edges.add_mirror_line({ bottom_mid, top_mid });
	edges.add_mirror_line({ bottom_right, top_right });
	edges.add_mirror_line({ top_left, top_right });
	edges.add_mirror_line({ center_left, center_right });
	edges.add_mirror_line({ bottom_left, bottom_right });
	edges.add_mirror_line({ top_left, bottom_right });
	edges.add_mirror_line({ bottom_left, top_right });

	return std::make_shared<WallpaperGroup>(
		"p4m",
		WallpaperGroup::SQUARE,
		cell,
		drawing_area,
		centers,
		edges,
		old
	);
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p4g(std::shared_ptr<SymmetryGroup> old) {
	SymbolicMatrix mirror = SymbolicMatrix::reflect({ center_left, bottom_mid });

	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	cell.push_back(SymbolicMatrix::rotate(90, origin));
	cell.push_back(SymbolicMatrix::rotate(180, origin));
	cell.push_back(SymbolicMatrix::rotate(270, origin));
	cell.push_back(mirror);
	cell.push_back(mirror * SymbolicMatrix::rotate(90, origin));
	cell.push_back(mirror * SymbolicMatrix::rotate(180, origin));
	cell.push_back(mirror * SymbolicMatrix::rotate(270, origin));

	std::vector<SymbolicPoint> drawing_area = {
		bottom_left, center_left, origin, bottom_mid
	};

	std::vector<SymbolicPoint> centers {
		top_left, top_mid, top_right,
		center_left, origin, center_right,
		bottom_left, bottom_mid, bottom_right,
	};

	DomainBoundaries edges;
	edges.add_mirror_line({ center_left, top_mid });
	edges.add_mirror_line({ top_mid, center_right });
	edges.add_mirror_line({ center_right, bottom_mid });
	edges.add_mirror_line({ bottom_mid, center_left });
	edges.add_type_A({ bottom_left, center_left });
	edges.add_type_A({ top_left, center_left });
	edges.add_type_A({ top_left, top_mid });
	edges.add_type_A({ top_right, top_mid });
	edges.add_type_A({ top_right, center_right });
	edges.add_type_A({ bottom_right, center_right });
	edges.add_type_A({ bottom_right, bottom_mid });
	edges.add_type_A({ bottom_left, bottom_mid });
	edges.add_type_A_mirror({ origin, center_left });
	edges.add_type_A_mirror({ origin, top_mid });
	edges.add_type_A_mirror({ origin, center_right });
	edges.add_type_A_mirror({ origin, bottom_mid });

	return std::make_shared<WallpaperGroup>(
		"p4g",
		WallpaperGroup::SQUARE,
		cell,
		drawing_area,
		centers,
		edges,
		old
	);
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p3(std::shared_ptr<SymmetryGroup> old) {
	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	cell.push_back(SymbolicMatrix::rotate(120, origin));
	cell.push_back(SymbolicMatrix::rotate(240, origin));

	std::vector<SymbolicPoint> drawing_area = {
		hex_p0, hex_p1, hex_p2, hex_p3, hex_p4, hex_p5
	};

	std::vector<SymbolicPoint> centers = {
		hex_p0, hex_p1, hex_p2, hex_p3, hex_p4, hex_p5, origin
	};

	DomainBoundaries edges;
	edges.add_type_A({ hex_p0, hex_p1 });
	edges.add_type_A({ hex_p2, hex_p1 });
	edges.add_type_A({ hex_p2, hex_p3 });
	edges.add_type_A({ hex_p4, hex_p3 });
	edges.add_type_A({ hex_p4, hex_p5 });
	edges.add_type_A({ hex_p0, hex_p5 });

	edges.add_type_B({ origin, hex_p0 });
	edges.add_type_B({ origin, hex_p2 });
	edges.add_type_B({ origin, hex_p4 });

	return std::make_shared<WallpaperGroup>(
		"p3",
		WallpaperGroup::HEXAGON,
		cell,
		drawing_area,
		centers,
		edges,
		old
	);
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p3m1(std::shared_ptr<SymmetryGroup> old) {
	SymbolicMatrix mirror = SymbolicMatrix::reflect({ hex_p0, hex_p3 });

	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	cell.push_back(SymbolicMatrix::rotate(120, origin));
	cell.push_back(SymbolicMatrix::rotate(240, origin));
	cell.push_back(mirror);
	cell.push_back(mirror * SymbolicMatrix::rotate(120, origin));
	cell.push_back(mirror * SymbolicMatrix::rotate(240, origin));

	std::vector<SymbolicPoint> drawing_area = {
		hex_p0, hex_p1, hex_p2, hex_p3, hex_p4, hex_p5
	};

	std::vector<SymbolicPoint> centers = {
		hex_p0, hex_p1, hex_p2, hex_p3, hex_p4, hex_p5, origin
	};

	DomainBoundaries edges;
	edges.add_mirror_line({ hex_p0, hex_p1 });
	edges.add_mirror_line({ hex_p1, hex_p2 });
	edges.add_mirror_line({ hex_p2, hex_p3 });
	edges.add_mirror_line({ hex_p3, hex_p4 });
	edges.add_mirror_line({ hex_p4, hex_p5 });
	edges.add_mirror_line({ hex_p5, hex_p0 });

	edges.add_mirror_line({ hex_p0, hex_p3 });
	edges.add_mirror_line({ hex_p2, hex_p5 });
	edges.add_mirror_line({ hex_p4, hex_p1 });

	return std::make_shared<WallpaperGroup>(
		"p3m1",
		WallpaperGroup::HEXAGON,
		cell,
		drawing_area,
		centers,
		edges,
		old
	);
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p31m(std::shared_ptr<SymmetryGroup> old) {
	SymbolicMatrix mirror = SymbolicMatrix::reflect({ hex_p0, hex_p4 });

	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	cell.push_back(SymbolicMatrix::rotate(120, origin));
	cell.push_back(SymbolicMatrix::rotate(240, origin));
	cell.push_back(mirror);
	cell.push_back(mirror * SymbolicMatrix::rotate(120, origin));
	cell.push_back(mirror * SymbolicMatrix::rotate(240, origin));

	std::vector<SymbolicPoint> drawing_area = {
		hex_p0, hex_p5, hex_p4, origin
	};

	std::vector<SymbolicPoint> centers = {
		hex_p0, hex_p1, hex_p2, hex_p3, hex_p4, hex_p5, origin
	};

	DomainBoundaries edges;
	edges.add_mirror_line({ hex_p0, hex_p2 });
	edges.add_mirror_line({ hex_p2, hex_p4 });
	edges.add_mirror_line({ hex_p4, hex_p0 });
	edges.add_type_A({ hex_p0, hex_p1 });
	edges.add_type_A({ hex_p2, hex_p1 });
	edges.add_type_A({ hex_p2, hex_p3 });
	edges.add_type_A({ hex_p4, hex_p3 });
	edges.add_type_A({ hex_p4, hex_p5 });
	edges.add_type_A({ hex_p0, hex_p5 });
	edges.add_type_A_mirror({ hex_p0, origin });
	edges.add_type_A_mirror({ hex_p2, origin });
	edges.add_type_A_mirror({ hex_p4, origin });

	return std::make_shared<WallpaperGroup>(
		"p31m",
		WallpaperGroup::HEXAGON,
		cell,
		drawing_area,
		centers,
		edges,
		old
	);
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p6(std::shared_ptr<SymmetryGroup> old) {
	SymbolicMatrix flip = SymbolicMatrix::rotate(180, hex_p8);

	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	cell.push_back(SymbolicMatrix::rotate(120, origin));
	cell.push_back(SymbolicMatrix::rotate(240, origin));
	cell.push_back(flip);
	cell.push_back(flip * SymbolicMatrix::rotate(120, origin));
	cell.push_back(flip * SymbolicMatrix::rotate(240, origin));

	std::vector<SymbolicPoint> drawing_area = {
		hex_p0, hex_p5, hex_p4, origin
	};

	std::vector<SymbolicPoint> centers = {
		hex_p0, hex_p1, hex_p2, hex_p3, hex_p4, hex_p5, hex_p6, hex_p7, hex_p8, origin
	};

	DomainBoundaries edges;
	edges.add_type_A({ hex_p0, hex_p1 });
	edges.add_type_A({ hex_p2, hex_p1 });
	edges.add_type_A({ hex_p2, hex_p3 });
	edges.add_type_A({ hex_p4, hex_p3 });
	edges.add_type_A({ hex_p4, hex_p5 });
	edges.add_type_A({ hex_p0, hex_p5 });

	edges.add_type_A({ hex_p0, origin });
	edges.add_type_A({ hex_p2, origin });
	edges.add_type_A({ hex_p4, origin });

	edges.add_type_B({ hex_p0, hex_p6 });
	edges.add_type_B({ hex_p2, hex_p6 });
	edges.add_type_B({ hex_p2, hex_p7 });
	edges.add_type_B({ hex_p4, hex_p7 });
	edges.add_type_B({ hex_p4, hex_p8 });
	edges.add_type_B({ hex_p0, hex_p8 });

	return std::make_shared<WallpaperGroup>(
		"p6",
		WallpaperGroup::HEXAGON,
		cell,
		drawing_area,
		centers,
		edges,
		old
	);
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p6m(std::shared_ptr<SymmetryGroup> old) {
	SymbolicMatrix mirror_one = SymbolicMatrix::reflect({ hex_p0, hex_p4 });
	SymbolicMatrix mirror_two = SymbolicMatrix::reflect({ origin, hex_p5 });

	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	cell.push_back(SymbolicMatrix::rotate(120, origin));
	cell.push_back(SymbolicMatrix::rotate(240, origin));
	cell.push_back(mirror_one);
	cell.push_back(mirror_one * SymbolicMatrix::rotate(120, origin));
	cell.push_back(mirror_one * SymbolicMatrix::rotate(240, origin));
	cell.push_back(mirror_two);
	cell.push_back(mirror_two * SymbolicMatrix::rotate(120, origin));
	cell.push_back(mirror_two * SymbolicMatrix::rotate(240, origin));
	cell.push_back(mirror_one * mirror_two);
	cell.push_back(mirror_one * mirror_two * SymbolicMatrix::rotate(120, origin));
	cell.push_back(mirror_one * mirror_two * SymbolicMatrix::rotate(240, origin));

	std::vector<SymbolicPoint> drawing_area = {
		hex_p0, hex_p5, hex_p4, origin
	};

	std::vector<SymbolicPoint> centers = {
		hex_p0, hex_p1, hex_p2, hex_p3, hex_p4, hex_p5, hex_p6, hex_p7, hex_p8, origin
	};

	DomainBoundaries edges;
	edges.add_mirror_line({ hex_p0, hex_p1 });
	edges.add_mirror_line({ hex_p1, hex_p2 });
	edges.add_mirror_line({ hex_p2, hex_p3 });
	edges.add_mirror_line({ hex_p3, hex_p4 });
	edges.add_mirror_line({ hex_p4, hex_p5 });
	edges.add_mirror_line({ hex_p5, hex_p0 });

	edges.add_mirror_line({ hex_p0, hex_p2 });
	edges.add_mirror_line({ hex_p2, hex_p4 });
	edges.add_mirror_line({ hex_p4, hex_p0 });

	edges.add_mirror_line({ hex_p0, hex_p3 });
	edges.add_mirror_line({ hex_p1, hex_p4 });
	edges.add_mirror_line({ hex_p2, hex_p5 });

	return std::make_shared<WallpaperGroup>(
		"p6m",
		WallpaperGroup::HEXAGON,
		cell,
		drawing_area,
		centers,
		edges,
		old
	);
}

std::shared_ptr<SymmetryGroup> SymmetryGroupFactory::from_json(json symm_json) {
	std::string type = symm_json["type"];
	if (type == "TrivialGroup") {
		return trivial();
	}
	if (type == "WallpaperGroup") {
		std::shared_ptr<WallpaperGroup> wallpaper_group;
		std::string name = symm_json["name"];	// Which of the seventeen wallpaper groups?
		if (name == "p1")
			wallpaper_group = p1(nullptr);
		else if (name == "p2")
			wallpaper_group = p2(nullptr);
		else if (name == "pm")
			wallpaper_group = pm(nullptr);
		else if (name == "pg")
			wallpaper_group = pg(nullptr);
		else if (name == "cm")
			wallpaper_group = cm(nullptr);
		else if (name == "pmm")
			wallpaper_group = pmm(nullptr);
		else if (name == "pmg")
			wallpaper_group = pmg(nullptr);
		else if (name == "pgg")
			wallpaper_group = pgg(nullptr);
		else if (name == "cmm")
			wallpaper_group = cmm(nullptr);
		else if (name == "p4")
			wallpaper_group = p4(nullptr);
		else if (name == "p4m")
			wallpaper_group = p4m(nullptr);
		else if (name == "p4g")
			wallpaper_group = p4g(nullptr);
		else if (name == "p3")
			wallpaper_group = p3(nullptr);
		else if (name == "p3m1")
			wallpaper_group = p3m1(nullptr);
		else if (name == "p31m")
			wallpaper_group = p31m(nullptr);
		else if (name == "p6")
			wallpaper_group = p6(nullptr);
		else if (name == "p6m")
			wallpaper_group = p6m(nullptr);
		else {
			std::string error_message = "error: unknown wallpaper group " + name;
			throw std::exception(error_message.c_str());
		}
		wallpaper_group->set_v1(symm_json["v1_x"], symm_json["v1_y"]);
		wallpaper_group->set_v2(symm_json["v2_x"], symm_json["v2_y"]);
		wallpaper_group->set_extent(symm_json["extent"]);
		return wallpaper_group;
	}
	else {
		std::string error_message = "unknown symmetry group type " + type;
		throw std::exception(error_message.c_str());
	}
}

