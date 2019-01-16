#include "ssd_iplugin.h"

/*
below layers must implement plugin in TensorRT-4.0.1.6
enum class PluginType : int
{
  kFASTERRCNN = 0,  //FasterRCNN fused plugin (RPN + ROI pooling)
  kNORMALIZE = 1,  //Normalize plugin
  kPERMUTE = 2, //Permute plugin
  kPRIORBOX = 3,  //PriorBox plugin
  kSSDDETECTIONOUTPUT = 4,  //SSD DetectionOutput plugin
  kConcat = 5,  //Concat plugin
  kPRELU = 6, //YOLO PReLU plugin
  kYOLOREORG = 7, //YOLO Reorg plugin
  kYOLOREGION = 8,  //YOLO Region plugin
  kANCHORGENERATOR = 9 //SSD Grid Anchor Generator
};
*/

nvinfer1::IPlugin* createPlugin(const char* layerName, const nvinfer1::Weights* weights, int nbWeights)
{
  assert(isPlugin(layerName));
  PriorBoxParameters prior_box_param;
  prior_box_param.numMinSize = 1;
  prior_box_param.numMaxSize = 1;
  prior_box_param.flip = true;
  prior_box_param.clip = false;
  prior_box_param.variance[0] = 0.1f;
  prior_box_param.variance[1] = 0.1f;
  prior_box_param.variance[2] = 0.2f;
  prior_box_param.variance[3] = 0.2f;
  prior_box_param.imgH = 0;
  prior_box_param.imgW = 0;
  prior_box_param.offset = 0.5f;
  DetectionOutputParameters detection_output_param;
  detection_output_param.shareLocation = true;
  detection_output_param.varianceEncodeedInTarget = false;
  detection_output_param.backgroundLabelId = 0;
  detection_output_param.numClasses = 21;
  detection_output_param.topK = 400;
  detection_output_param.keepTopK = 200;
  detection_output_param.confidenceThreshold = 0.1f;
  detection_output_param.nmsThreshold = 0.45f;
  detection_output_param.inputOrder[0] = 0;
  detection_output_param.inputOrder[1] = 1;
  detection_output_param.inputOrder[2] = 2;
  detection_output_param.confSigmoid = false;
  detection_output_param.isNormalized = true;
  Quadruple permute_param;
  permute_param.data[0] = 0;
  permute_param.data[1] = 2;
  permute_param.data[2] = 3;
  permute_param.data[3] = 1;
  //normalize layer
  if (!strcmp(layerName, "conv4_3_norm"))
  {
    assert(mNormalizeLayer == nullptr);
    bool acrossSpatial = false, channelShared = false;
    float eps = 0.0001;
    mNormalizeLayer = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createSSDNormalizePlugin(weights, acrossSpatial, channelShared, eps), nvPluginDeleter);
    return mNormalizeLayer.get();
  }
  //priorbox layer
  else if (!strcmp(layerName, "conv4_3_norm_mbox_priorbox"))
  {
    assert(mConv4_3_norm_mbox_priorbox_layer == nullptr);
    float min_size = 30.0f, max_size = 60.0f, aspect_ratio[2] = {1.0f, 2.0f};
    prior_box_param.min_size = &min_size;
    prior_box_param.max_size = &max_size;
    prior_box_param.aspectRatios = aspect_ratio;
    prior_box_param.numAspectRatios = 2;
    prior_box_param.stepH = 8.0f;
    prior_box_param.stepW = 8.0f;
    mConv4_3_norm_mbox_priorbox_layer = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createSSDPriorBoxPlugin(prior_box_param), nvPluginDeleter);
    return mConv4_3_norm_mbox_priorbox_layer.get();
  }
  else if (!strcmp(layerName, "fc7_mbox_priorbox"))
  {
    assert(mFc7_mbox_priorbox_layer == nullptr);
    float min_size = 60.0f, max_size = 111.0f, aspect_ratio[3] = {1.0f, 2.0f, 3.0f};
    prior_box_param.min_size = &min_size;
    prior_box_param.max_size = &max_size;
    prior_box_param.aspectRatios = aspect_ratio;
    prior_box_param.numAspectRatios = 3;
    prior_box_param.stepH = 16.0f;
    prior_box_param.stepW = 16.0f;
    mFc7_mbox_priorbox_layer = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createSSDPriorBoxPlugin(prior_box_param), nvPluginDeleter);
    return mFc7_mbox_priorbox_layer.get();
  }
  else if (!strcmp(layerName, "conv6_2_mbox_priorbox"))
  {
    assert(mConv6_2_mbox_priorbox_layer == nullptr);
    float min_size = 111.0f, max_size = 162.0f, aspect_ratio[3] = {1.0f, 2.0f, 3.0f};
    prior_box_param.min_size = &min_size;
    prior_box_param.max_size = &max_size;
    prior_box_param.aspectRatios = aspect_ratio;
    prior_box_param.numAspectRatios = 3;
    prior_box_param.stepH = 32.0f;
    prior_box_param.stepW = 32.0f;
    mConv6_2_mbox_priorbox_layer = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createSSDPriorBoxPlugin(prior_box_param), nvPluginDeleter);
    return mConv6_2_mbox_priorbox_layer.get();
  }
  else if (!strcmp(layerName, "conv7_2_mbox_priorbox"))
  {
    assert(mConv7_2_mbox_priorbox_layer == nullptr);
    float min_size = 162.0f, max_size = 213.0f, aspect_ratio[3] = {1.0f, 2.0f, 3.0f};
    prior_box_param.min_size = &min_size;
    prior_box_param.max_size = &max_size;
    prior_box_param.aspectRatios = aspect_ratio;
    prior_box_param.numAspectRatios = 3;
    prior_box_param.stepH = 64.0f;
    prior_box_param.stepW = 64.0f;
    mConv7_2_mbox_priorbox_layer = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createSSDPriorBoxPlugin(prior_box_param), nvPluginDeleter);
    return mConv7_2_mbox_priorbox_layer.get();
  }
  else if (!strcmp(layerName, "conv8_2_mbox_priorbox"))
  {
    assert(mConv8_2_mbox_priorbox_layer == nullptr);
    float min_size = 213.0f, max_size = 264.0f, aspect_ratio[2] = {1.0f, 2.0f};
    prior_box_param.min_size = &min_size;
    prior_box_param.max_size = &max_size;
    prior_box_param.aspectRatios = aspect_ratio;
    prior_box_param.numAspectRatios = 2;
    prior_box_param.stepH = 100.0f;
    prior_box_param.stepW = 100.0f;
    mConv8_2_mbox_priorbox_layer = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createSSDPriorBoxPlugin(prior_box_param), nvPluginDeleter);
    return mConv8_2_mbox_priorbox_layer.get();
  }
  else if (!strcmp(layerName, "conv9_2_mbox_priorbox"))
  {
    assert(mConv9_2_mbox_priorbox_layer == nullptr);
    float min_size = 264.0f, max_size = 315.0f, aspect_ratio[2] = {1.0f, 2.0f};
    prior_box_param.min_size = &min_size;
    prior_box_param.max_size = &max_size;
    prior_box_param.aspectRatios = aspect_ratio;
    prior_box_param.numAspectRatios = 2;
    prior_box_param.stepH = 300.0f;
    prior_box_param.stepW = 300.0f;
    mConv9_2_mbox_priorbox_layer = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createSSDPriorBoxPlugin(prior_box_param), nvPluginDeleter);
    return mConv9_2_mbox_priorbox_layer.get();
  }
  //detection output layer
  else if (!strcmp(layerName, "detection_out"))
  {
    assert(mDetection_out == nullptr);
    mDetection_out = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createSSDDetectionOutputPlugin(detection_output_param), nvPluginDeleter);
    return mDetection_out.get();
  }
  //permute layers
  else if (!strcmp(layerName, "conv4_3_norm_mbox_loc_perm"))
  {
    assert(mConv4_3_norm_mbox_loc_permute_layer == nullptr);
    mConv4_3_norm_mbox_loc_permute_layer = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createSSDPermutePlugin(permute_param), nvPluginDeleter);
    return mConv4_3_norm_mbox_loc_permute_layer.get();
  }
  else if (!strcmp(layerName, "conv4_3_norm_mbox_conf_perm"))
  {
    assert(mConv4_3_norm_mbox_conf_permute_layer == nullptr);
    mConv4_3_norm_mbox_conf_permute_layer = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createSSDPermutePlugin(permute_param), nvPluginDeleter);
    return mConv4_3_norm_mbox_conf_permute_layer.get();
  }
  else if (!strcmp(layerName, "fc7_mbox_loc_perm"))
  {
    assert(mFc7_mbox_loc_permute_layer == nullptr);
    mFc7_mbox_loc_permute_layer = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createSSDPermutePlugin(permute_param), nvPluginDeleter);
    return mFc7_mbox_loc_permute_layer.get();
  }
  else if (!strcmp(layerName, "fc7_mbox_conf_perm"))
  {
    assert(mFc7_mbox_conf_permute_layer == nullptr);
    mFc7_mbox_conf_permute_layer = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createSSDPermutePlugin(permute_param), nvPluginDeleter);
    return mFc7_mbox_conf_permute_layer.get();
  }
  else if (!strcmp(layerName, "conv6_2_mbox_loc_perm"))
  {
    assert(mConv6_2_mbox_loc_permute_layer == nullptr);
    mConv6_2_mbox_loc_permute_layer = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createSSDPermutePlugin(permute_param), nvPluginDeleter);
    return mConv6_2_mbox_loc_permute_layer.get();
  }
  else if (!strcmp(layerName, "conv6_2_mbox_conf_perm"))
  {
    assert(mConv6_2_mbox_conf_permute_layer == nullptr);
    mConv6_2_mbox_conf_permute_layer = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createSSDPermutePlugin(permute_param), nvPluginDeleter);
    return mConv6_2_mbox_conf_permute_layer.get();
  }
  else if (!strcmp(layerName, "conv7_2_mbox_loc_perm"))
  {
    assert(mConv7_2_mbox_loc_permute_layer == nullptr);
    mConv7_2_mbox_loc_permute_layer = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createSSDPermutePlugin(permute_param), nvPluginDeleter);
    return mConv7_2_mbox_loc_permute_layer.get();
  }
  else if (!strcmp(layerName, "conv7_2_mbox_conf_perm"))
  {
    assert(mConv7_2_mbox_conf_permute_layer == nullptr);
    mConv7_2_mbox_conf_permute_layer = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createSSDPermutePlugin(permute_param), nvPluginDeleter);
    return mConv7_2_mbox_conf_permute_layer.get();
  }
  else if (!strcmp(layerName, "conv8_2_mbox_loc_perm"))
  {
    assert(mConv8_2_mbox_loc_permute_layer == nullptr);
    mConv8_2_mbox_loc_permute_layer = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createSSDPermutePlugin(permute_param), nvPluginDeleter);
    return mConv8_2_mbox_loc_permute_layer.get();
  }
  else if (!strcmp(layerName, "conv8_2_mbox_conf_perm"))
  {
    assert(mConv8_2_mbox_conf_permute_layer == nullptr);
    mConv8_2_mbox_conf_permute_layer = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createSSDPermutePlugin(permute_param), nvPluginDeleter);
    return mConv8_2_mbox_conf_permute_layer.get();
  }
  else if (!strcmp(layerName, "conv9_2_mbox_loc_perm"))
  {
    assert(mConv9_2_mbox_loc_permute_layer == nullptr);
    mConv9_2_mbox_loc_permute_layer = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createSSDPermutePlugin(permute_param), nvPluginDeleter);
    return mConv9_2_mbox_loc_permute_layer.get();
  }
  else if (!strcmp(layerName, "conv9_2_mbox_conf_perm"))
  {
    assert(mConv9_2_mbox_conf_permute_layer == nullptr);
    mConv9_2_mbox_conf_permute_layer = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createSSDPermutePlugin(permute_param), nvPluginDeleter);
    return mConv9_2_mbox_conf_permute_layer.get();
  }
  //concat layers
  else if (!strcmp(layerName, "mbox_loc"))
  {
    assert(mMbox_loc_concat_layer == nullptr);
    mMbox_loc_concat_layer = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createConcatPlugin(1, true), nvPluginDeleter);
    return mMbox_loc_concat_layer.get();
  }
  else if (!strcmp(layerName, "mbox_conf"))
  {
    assert(mMbox_conf_concat_layer == nullptr);
    mMbox_conf_concat_layer = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createConcatPlugin(1, true), nvPluginDeleter);
    return mMbox_conf_concat_layer.get();
  }
  else if (!strcmp(layerName, "mbox_priorbox"))
  {
    assert(mMbox_priorbox_concat_layer == nullptr);
    mMbox_priorbox_concat_layer = std::unique_ptr<INvPlugin, decltype(nvPluginDeleter)>(createConcatPlugin(2, true), nvPluginDeleter);
    return mMbox_priorbox_concat_layer.get();
  }
  //reshape layer
  else if (!strcmp(layerName, "mbox_conf_reshape"))
  {
    assert(mMbox_conf_reshape_layer == nullptr);
    mMbox_conf_reshape_layer = std::unique_ptr<Reshape<21>>(new Reshape<21>());
    return mMbox_conf_reshape_layer.get();
  }
  //flatten layers
  else if (!strcmp(layerName, "conv4_3_norm_mbox_loc_flat"))
  {
    assert(mConv4_3_norm_mbox_loc_flatten_layer == nullptr);
    mConv4_3_norm_mbox_loc_flatten_layer = std::unique_ptr<Flatten>(new Flatten());
    return mConv4_3_norm_mbox_loc_flatten_layer.get();
  }
  else if (!strcmp(layerName, "conv4_3_norm_mbox_conf_flat"))
  {
    assert(mConv4_3_norm_mbox_conf_flatten_layer == nullptr);
    mConv4_3_norm_mbox_conf_flatten_layer = std::unique_ptr<Flatten>(new Flatten());
    return mConv4_3_norm_mbox_conf_flatten_layer.get();
  }
  else if (!strcmp(layerName, "fc7_mbox_loc_flat"))
  {
    assert(mFc7_mbox_loc_flatten_layer == nullptr);
    mFc7_mbox_loc_flatten_layer = std::unique_ptr<Flatten>(new Flatten());
    return mFc7_mbox_loc_flatten_layer.get();
  }
  else if (!strcmp(layerName, "fc7_mbox_conf_flat"))
  {
    assert(mFc7_mbox_conf_flatten_layer == nullptr);
    mFc7_mbox_conf_flatten_layer = std::unique_ptr<Flatten>(new Flatten());
    return mFc7_mbox_conf_flatten_layer.get();
  }
  else if (!strcmp(layerName, "conv6_2_mbox_loc_flat"))
  {
    assert(mConv6_2_mbox_loc_flatten_layer == nullptr);
    mConv6_2_mbox_loc_flatten_layer = std::unique_ptr<Flatten>(new Flatten());
    return mConv6_2_mbox_loc_flatten_layer.get();
  }
  else if (!strcmp(layerName, "conv6_2_mbox_conf_flat"))
  {
    assert(mConv6_2_mbox_conf_flatten_layer == nullptr);
    mConv6_2_mbox_conf_flatten_layer = std::unique_ptr<Flatten>(new Flatten());
    return mConv6_2_mbox_conf_flatten_layer.get();
  }
  else if (!strcmp(layerName, "conv7_2_mbox_loc_flat"))
  {
    assert(mConv7_2_mbox_loc_flatten_layer == nullptr);
    mConv7_2_mbox_loc_flatten_layer = std::unique_ptr<Flatten>(new Flatten());
    return mConv7_2_mbox_loc_flatten_layer.get();
  }
  else if (!strcmp(layerName, "conv7_2_mbox_conf_flat"))
  {
    assert(mConv7_2_mbox_conf_flatten_layer == nullptr);
    mConv7_2_mbox_conf_flatten_layer = std::unique_ptr<Flatten>(new Flatten());
    return mConv7_2_mbox_conf_flatten_layer.get();
  }
  else if (!strcmp(layerName, "conv8_2_mbox_loc_flat"))
  {
    assert(mConv8_2_mbox_loc_flatten_layer == nullptr);
    mConv8_2_mbox_loc_flatten_layer = std::unique_ptr<Flatten>(new Flatten());
    return mConv8_2_mbox_loc_flatten_layer.get();
  }
  else if (!strcmp(layerName, "conv8_2_mbox_conf_flat"))
  {
    assert(mConv8_2_mbox_conf_flatten_layer == nullptr);
    mConv8_2_mbox_conf_flatten_layer = std::unique_ptr<Flatten>(new Flatten());
    return mConv8_2_mbox_conf_flatten_layer.get();
  }
  else if (!strcmp(layerName, "conv9_2_mbox_loc_flat"))
  {
    assert(mConv9_2_mbox_loc_flatten_layer == nullptr);
    mConv9_2_mbox_loc_flatten_layer = std::unique_ptr<Flatten>(new Flatten());
    return mConv9_2_mbox_loc_flatten_layer.get();
  }
  else if (!strcmp(layerName, "conv9_2_mbox_conf_flat"))
  {
    assert(mConv9_2_mbox_conf_flatten_layer == nullptr);
    mConv9_2_mbox_conf_flatten_layer = std::unique_ptr<Flatten>(new Flatten());
    return mConv9_2_mbox_conf_flatten_layer.get();
  }
  
  
  
  
  
  
  
  
}
