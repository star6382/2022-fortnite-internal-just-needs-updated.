#pragma once 

    // Define bone positions
    Vector3 bottom, bottom_2, head, head_2, neck, chest, pelvis, rshoulder, relbow, rhand, rthigh, rknee, rfoot, rfootout;
    Vector2 s_bottom, s_bottom_2, s_head, s_head_2, s_neck, s_chest, s_pelvis, s_rshoulder, s_relbow, s_rhand, s_rthigh, s_rknee, s_rfoot, s_rfootout;

    bool GetBoneID(APlayerController* controller, AFortPawn* player) {
        bool success = true;

        // Get bone locations
        bottom = player->GetBoneLocation(0);
        bottom_2 = bottom; bottom_2.z -= 8;
        head = player->GetBoneLocation(68);
        head_2 = head; head_2.z += 20;
        neck = player->GetBoneLocation(67);
        chest = player->GetBoneLocation(7); chest.z += 7;
        pelvis = player->GetBoneLocation(2);
        rshoulder = player->GetBoneLocation(38);
        relbow = player->GetBoneLocation(39);
        rhand = player->GetBoneLocation(40);
        rthigh = player->GetBoneLocation(78);
        rknee = player->GetBoneLocation(79);
        rfoot = player->GetBoneLocation(82);
        rfootout = player->GetBoneLocation(83);

        // Convert world coordinates to screen coordinates
        if (success && !controller->w2s(bottom, &s_bottom)) success = false;
        if (success && !controller->w2s(bottom_2, &s_bottom_2)) success = false;
        if (success && !controller->w2s(head, &s_head)) success = false;
        if (success && !controller->w2s(head_2, &s_head_2)) success = false;
        if (success && !controller->w2s(neck, &s_neck)) success = false;
        if (success && !controller->w2s(chest, &s_chest)) success = false;
        if (success && !controller->w2s(pelvis, &s_pelvis)) success = false;
        if (success && !controller->w2s(rshoulder, &s_rshoulder)) success = false;
        if (success && !controller->w2s(relbow, &s_relbow)) success = false;
        if (success && !controller->w2s(rhand, &s_rhand)) success = false;
        if (success && !controller->w2s(rthigh, &s_rthigh)) success = false;
        if (success && !controller->w2s(rknee, &s_rknee)) success = false;
        if (success && !controller->w2s(rfoot, &s_rfoot)) success = false;
        if (success && !controller->w2s(rfootout, &s_rfootout)) success = false;

        return success;
    }