#include "draco_point_cloud_transport/PC2toDraco.h"
#include "debug_msg.h"

//! Constructor
PC2toDraco::PC2toDraco(sensor_msgs::PointCloud2 PC2)
{
    PC2_ = PC2;

    const char *PointFieldDataTypes[] =
            {
                    "Undefined",
                    "INT8",
                    "UINT8",
                    "INT16",
                    "UINT16",
                    "INT32",
                    "UINT32",
                    "FLOAT32",
                    "FLOAT64"
            };

    Initialize();
}

//! Destructor
//PC2toDraco::~PC2toDraco(){}

//! Method for converting into Draco pointcloud using draco::PointCloudBuilder
std::unique_ptr<draco::PointCloud> PC2toDraco::convert()
{
    // object for conversion into Draco Point Cloud format
    draco::PointCloudBuilder builder;
    // number of points in point cloud
    uint64_t number_of_points = PC2_.height * PC2_.width;
    // initialize builder object, requires prior knowledge of point cloud size for buffer allocation
    builder.Start(number_of_points);
    // vector to hold IDs of attributes for builder object
    std::vector<int> att_ids;
    // set to 0 if invalid datatype (not in enum) is encountered in PointField
    bool ValidDataType;

    // fill in att_ids with attributes from PointField[] fields
    for (sensor_msgs::PointField field : PC2_.fields) {

        ValidDataType=1;

        //! strings mapped to enumeration values; used for switches
        switch (s_mapStringValues[field.name]) {
            case enumval1 : // "x"
            case enumval2 : // "y"
            case enumval3 : // "z"
            case enumval4 : // "pos"
            case enumval5 : // "position"
                switch (field.datatype) {
                    case 1 :
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::POSITION, field.count, draco::DT_INT8));
                        break;
                    case 2 :
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::POSITION, field.count, draco::DT_UINT8));
                        break;
                    case 3 :
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::POSITION, field.count, draco::DT_INT16));
                        break;
                    case 4 :
                        att_ids.push_back(builder.AddAttribute(draco::GeometryAttribute::POSITION, field.count,
                                                                  draco::DT_UINT16));
                        break;
                    case 5 :
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::POSITION, field.count, draco::DT_INT32));
                        break;
                    case 6 :
                        att_ids.push_back(builder.AddAttribute(draco::GeometryAttribute::POSITION, field.count,
                                                                  draco::DT_UINT32));
                        break;
                    case 7 :
                        att_ids.push_back(builder.AddAttribute(draco::GeometryAttribute::POSITION, field.count,
                                                                  draco::DT_FLOAT32));
                        break;
                    case 8 :
                        att_ids.push_back(builder.AddAttribute(draco::GeometryAttribute::POSITION, field.count,
                                                                  draco::DT_FLOAT64));
                        break;
                    default:
                        // RAISE ERROR - INVALID DATA TYPE
                        ROS_FATAL_STREAM(" INVALID DATA TYPE in PC2toDraco conversion - sensor_msgs::PointField::datatype");
                        ValidDataType=0;
                        break;
                }
                break;

            case enumval6 : // "r"
            case enumval7 : // "g"
            case enumval8 : // "b"
            case enumval9 : // "a"
                switch (field.datatype) {
                    case 1 :
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::COLOR, field.count, draco::DT_INT8));
                        break;
                    case 2 :
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::COLOR, field.count, draco::DT_UINT8));
                        break;
                    case 3 :
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::COLOR, field.count, draco::DT_INT16));
                        break;
                    case 4 :
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::COLOR, field.count, draco::DT_UINT16));
                        break;
                    case 5 :
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::COLOR, field.count, draco::DT_INT32));
                        break;
                    case 6 :
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::COLOR, field.count, draco::DT_UINT32));
                        break;
                    case 7 :
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::COLOR, field.count, draco::DT_FLOAT32));
                        break;
                    case 8 :
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::COLOR, field.count, draco::DT_FLOAT64));
                        break;
                    default:
                        // RAISE ERROR - INVALID DATA TYPE
                        ROS_FATAL_STREAM(" INVALID DATA TYPE in PC2toDraco conversion - sensor_msgs::PointField::datatype");
                        ValidDataType=0;
                        break;
                }
                break;

            // apply the common ROS tweaks
            case enumval10 : // "rgb"
            case enumval11 : // "rgba"
                switch (field.datatype) {
                    case 5 : // DT_INT32
                    case 6 : // DT_UINT32
                    case 7 : // DT_FLOAT32
                    // TODO: theoretically, setting data type to be 4 times smaller and count to be 4 times larger should work
                    // TODO: this implementation needs to be tested however and husky.bag point cloud does not include COLOR
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::COLOR, 4 * field.count, draco::DT_UINT8));
                        break;
                    case 8 : // DT_FLOAT64
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::COLOR, 4 * field.count, draco::DT_UINT16));
                        break;
                    default:
                        // RAISE ERROR - INVALID DATA TYPE
                        ROS_FATAL_STREAM(" INVALID DATA TYPE in PC2toDraco conversion - sensor_msgs::PointField::datatype");
                        ValidDataType=0;
                        break;
                }
                break;


            case enumval12 : // "nx"
            case enumval13 : // "ny"
            case enumval14 : // "nz"
                switch (field.datatype) {
                    case 1 :
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::NORMAL, field.count, draco::DT_INT8));
                        break;
                    case 2 :
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::NORMAL, field.count, draco::DT_UINT8));
                        break;
                    case 3 :
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::NORMAL, field.count, draco::DT_INT16));
                        break;
                    case 4 :
                        att_ids.push_back(builder.AddAttribute(draco::GeometryAttribute::NORMAL, field.count,
                                                               draco::DT_UINT16));
                        break;
                    case 5 :
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::NORMAL, field.count, draco::DT_INT32));
                        break;
                    case 6 :
                        att_ids.push_back(builder.AddAttribute(draco::GeometryAttribute::NORMAL, field.count,
                                                               draco::DT_UINT32));
                        break;
                    case 7 :
                        att_ids.push_back(builder.AddAttribute(draco::GeometryAttribute::NORMAL, field.count,
                                                               draco::DT_FLOAT32));
                        break;
                    case 8 :
                        att_ids.push_back(builder.AddAttribute(draco::GeometryAttribute::NORMAL, field.count,
                                                               draco::DT_FLOAT64));
                        break;
                    default:
                        // RAISE ERROR - INVALID DATA TYPE
                        ROS_FATAL_STREAM(" INVALID DATA TYPE in PC2toDraco conversion - sensor_msgs::PointField::datatype");
                        ValidDataType=0;
                        break;
                }
                break;

                // default for unknown data
            case enumvalGeneric :
                switch (field.datatype) {
                    case 1 :
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::GENERIC, field.count, draco::DT_INT8));
                        break;
                    case 2 :
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::GENERIC, field.count, draco::DT_UINT8));
                        break;
                    case 3 :
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::GENERIC, field.count, draco::DT_INT16));
                        break;
                    case 4 :
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::GENERIC, field.count, draco::DT_UINT16));
                        break;
                    case 5 :
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::GENERIC, field.count, draco::DT_INT32));
                        break;
                    case 6 :
                        att_ids.push_back(
                                builder.AddAttribute(draco::GeometryAttribute::GENERIC, field.count, draco::DT_UINT32));
                        break;
                    case 7 :
                        att_ids.push_back(builder.AddAttribute(draco::GeometryAttribute::GENERIC, field.count,
                                                                  draco::DT_FLOAT32));
                        break;
                    case 8 :
                        att_ids.push_back(builder.AddAttribute(draco::GeometryAttribute::GENERIC, field.count,
                                                                  draco::DT_FLOAT64));
                        break;
                    default:
                        // RAISE ERROR - INVALID DATA TYPE
                        ROS_FATAL_STREAM(" INVALID DATA TYPE in PC2toDraco conversion - sensor_msgs::PointField::datatype");
                        ValidDataType=0;
                        break;
                }
        }

        // Set attribute values for the last added attribute
        if (ValidDataType and !att_ids.empty()) {
             builder.SetAttributeValuesForAllPoints(int(att_ids.back()), &PC2_.data[0] + field.offset, PC2_.point_step);
            }
    }
    // finalize point cloud *** builder.Finalize(bool deduplicate) ***
    std::unique_ptr<draco::PointCloud> pc = builder.Finalize(false);

    // RAISE ERROR - INVALID DATA TYPE
    if (pc == nullptr)
    {
        ROS_FATAL_STREAM("Conversion from sensor_msgs::PointCloud2 to Draco::PointCloud failed");
    }

    if (pc->num_points()!=number_of_points)
    {
        ROS_WARN_STREAM("Number of points in Draco::PointCloud differs from sensor_msgs::PointCloud2. Did user set deduplication on?");
    }
    return pc;
}

